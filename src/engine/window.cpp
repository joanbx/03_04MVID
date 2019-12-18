#include "engine/input.hpp"
#include "engine/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void onChangeFrameBufferSize(GLFWwindow*, const int32_t width, const int32_t height) noexcept {
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else {
        Input::instance()->keyPressed(key, action);
    }
}

void onMouseMove(GLFWwindow* window, double x, double y) {
    Input::instance()->mouseMoved(x, y);
}

void onScrollMove(GLFWwindow* window, double x, double y) {
    Input::instance()->scrollMove(x, y);
}

Window::Window() {
    if (!glfwInit()) {
        std::cout << "Error Initializing GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(800, 600, "04MVID", nullptr, nullptr);
    if (!window_) {
        std::cout << "Error Creating Window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGL()) {
        std::cout << "Error Initializing GLAD" << std::endl;
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(window_, onChangeFrameBufferSize);
    glfwSetKeyCallback(window_, onKeyPress);
    glfwSetCursorPosCallback(window_, onMouseMove);
    glfwSetScrollCallback(window_, onScrollMove);

}

Window::~Window() {
    glfwTerminate();
}

Window* Window::instance() {
    static Window w;
    return &w;
}

bool Window::alive() const {
    return !glfwWindowShouldClose(window_);
}

void Window::frame() const {
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool Window::keyPressed(int key) {
    return glfwGetKey(window_, key) == GLFW_PRESS;
}

void Window::setCaptureMode(bool toggle) const {
    if (toggle) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
