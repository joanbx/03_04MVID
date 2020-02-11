#include <engine\particleSystem.hpp>

ParticleSystem::ParticleSystem(const Shader& shader, const Texture& texture, const Geometry& geometry, uint32_t amount)
	: _shader(shader), _texture(texture), _geometry(geometry), amount(amount)
{

}

void ParticleSystem::Start()
{
	//std::cout << "INIT PS" << std::endl;
	loop = 0;
	_scalemulti = 1.0f;
	_finish = false;
	expansionDirection = true;
	// Create amount default particle instances
	particles.clear();
	for (uint32_t i = 0; i < amount; ++i)
		particles.push_back(Particle());
}



void ParticleSystem::Update(float dt, uint32_t newParticles, GameObject& go, glm::vec3 offset)
{

	if (loop < maxLoop) {
		// Add new particles 
		_position = go.Position();
		for (uint32_t i = 0; i < newParticles; ++i)
		{
			int unusedParticle = firstUnusedParticle();
			respawnParticle(particles[unusedParticle], offset);
		}
		// Update all particles
		for (uint32_t i = 0; i < amount; ++i)
		{
			Particle& p = particles[i];
			p.Life -= dt; // reduce life
			if (p.Life > 0.0f)
			{	// particle is alive, thus update
				p.Position -= p.Velocity * dt;
				p.Color.a -= dt * 2.5;
			}
		}
		if (_scalemulti > 5 && expansionDirection) { expansionDirection = false; }
		else if (_scalemulti <= 1.0 && !expansionDirection) { expansionDirection = true; loop++; }

		std::cout << _scalemulti << std::endl;
		if (expansionDirection) _scalemulti *= (1+(5.0f * dt));
		else _scalemulti /= (1+(5.0f * dt));

		Draw(go.getProj(), go.getView());
	}
	else {
		//std::cout << "FINISH" << std::endl;
		_finish = true;
	}
	

	
	//std::cout << _scalemulti << std::endl;
}

void ParticleSystem::setFinished(bool finish) {
	_finish = finish;
}

// Render all particles
void ParticleSystem::Draw(glm::mat4& proj, glm::mat4& view)
{
	// Use additive blending to give it a 'glow' effect
	if (!_finish) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		_shader.use();
		for (Particle particle : particles)
		{
			if (particle.Life > 0.0f)
			{
				_shader.use();
				//std::cout << "RENDER PS -> " << _position.x <<"," <<_position.y << ","<<_position.z << std::endl;
				_texture.use(_shader, "tex", 0);
				glm::mat4 model = glm::mat4(1.0);
				model = glm::translate(model, _position); //glm::vec3(particle.Position.x, particle.Position.y, particle.Position.z)
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				//model = glm::scale(model, particle.Scale);
				_shader.set("model", model);
				_shader.set("proj", proj);
				_shader.set("view", view);
				//std::cout << particle.Scale.x << " " << particle.Scale.y << " " << particle.Scale.z << std::endl;
				_shader.set("scale", particle.Scale);
				_shader.set("offset", particle.Position);
				_shader.set("color", glm::vec4(1, 1, 1, 1));

				_geometry.render();
			}
		}
		// Don't forget to reset to default blending mode

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
	}
	
}



// Stores the index of the last particle used (for quick access to next dead particle)
uint32_t lastUsedParticle = 0;
GLuint ParticleSystem::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (uint32_t i = lastUsedParticle; i < amount; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (uint32_t i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleSystem::respawnParticle(Particle& particle,  glm::vec3 offset)
{
	float random = ((double)rand() / (RAND_MAX)) + 1;
	float rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = offset*random;
	particle.Scale = _scalemulti;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec3(1.0f,1.0f,1.0f) * 0.1f;
}