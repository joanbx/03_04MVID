#include <glad\glad.h>
#include <engine\shader.hpp>
#include <engine\window.hpp>
#include <GLFW\glfw3.h>
#include <engine\textRenderer.hpp>

float lastFrame = 0.0f;

void render(TextRenderer& textRenderer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	textRenderer.RenderText("Lives:", 5.0f, 5.0f, 1.0f, glm::vec3(1.0f,0.0f,0.0f));

}
int main(int, char* []) {
    Window* window = Window::instance();
	window->setWidth(600);
	window->setHeight(800);

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_text("../projects/FINAL/text.vs", "../projects/FINAL/text.fs");
	TextRenderer textRenderer(s_text, 600, 800);
	textRenderer.Load("../assets/fonts/arial.ttf", 48);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        render(textRenderer);
       //render(n0, n1, quad, cube, s_phong, s_depth, s_debug, s_light, t_albedo, t_specular, t_normal);
        window->frame();
    }

    //glDeleteFramebuffers(1, &fbo.first);
    //glDeleteTextures(1, &fbo.second);

    return 0;
}