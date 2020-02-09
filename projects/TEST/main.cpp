#include <glad\glad.h>
#include <engine\shader.hpp>
#include <engine\window.hpp>
#include <GLFW\glfw3.h>
#include <engine\textRenderer.hpp>
#include <engine\geometry\quad.hpp>
#include <engine\particleSystem.hpp>

Camera camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0, -90);
float lastFrame = 0.0f;

void render(float dt, ParticleSystem& ps) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//textRenderer.RenderText("Lives:", 5.0f, 5.0f, 1.0f, glm::vec3(1.0f,0.0f,0.0f));
	//shader.use();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	//texture.use(shader, "tex", 0);
	//glm::mat4 model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(0, 0, 0));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	////model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//shader.set("model", model);
	//shader.set("view", camera.getViewMatrix());
	//shader.set("proj", camera.getProj());
	//
	//geometry.render();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ps._shader.use();
	ps._shader.set("view", camera.getViewMatrix());
	ps._shader.set("proj", camera.getProj());
	ps.Update(dt, 2, glm::vec3(0.1f, 0.1f, 0.1f));
	ps.Draw();
}
int main(int, char* []) {
    Window* window = Window::instance();
	window->setWidth(600);
	window->setHeight(800);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    //const Shader s_text("../projects/FINAL/text.vs", "../projects/FINAL/text.fs");
	//TextRenderer textRenderer(s_text, 600, 800);
	//textRenderer.Load("../assets/fonts/arial.ttf", 48);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const Shader s_particle("../projects/FINAL/particle.vs", "../projects/FINAL/particle.fs");
	const Texture t_particle("../assets/textures/explosion.png", Texture::Format::RGBA);
	const Quad quadTest(1.0f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	ParticleSystem ps(s_particle, t_particle, quadTest, 2);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        render(deltaTime, ps);
       //render(n0, n1, quad, cube, s_phong, s_depth, s_debug, s_light, t_albedo, t_specular, t_normal);
        window->frame();
    }

    //glDeleteFramebuffers(1, &fbo.first);
    //glDeleteTextures(1, &fbo.second);

    return 0;
}