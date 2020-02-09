#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H
#include <vector>

#include <glm/glm.hpp>

#include <engine\shader.hpp>
#include <engine\texture.hpp>
#include <engine\gameObject.hpp>


// Represents a single particle and its state
struct Particle {
	glm::vec3 Position,Velocity;
	glm::vec4 Color;
	float Life;
	float Scale;

	Particle() : Position(0.0f), Scale(0.1f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleSystem
{
public:
	const Shader& _shader;
	
	// Constructor
	ParticleSystem(const Shader& shader, const Texture& texture,  const Geometry& geometry, uint32_t amount);
	void Start();
	// Update all particles
	void Update(float dt,GLuint newParticles, GameObject& go,glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f));
	// Render all particles
	void Draw(glm::mat4& proj, glm::mat4& view);

	bool getFinished() { return _finish; }
	void setFinished(bool finish);
private:
	// State
	std::vector<Particle> particles;
	const Texture& _texture;
	const Geometry& _geometry;
	uint32_t amount;
	float _scalemulti = 0.0f;
	bool expansionDirection = true;
	uint32_t loop = 0;
	uint32_t maxLoop = 1;
	bool _finish = false;
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	// Initializes buffer and vertex attributes
	
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f));

};

#endif