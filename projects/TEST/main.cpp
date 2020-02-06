#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"
#include "engine/geometry/quad.hpp"
#include <iostream>
#include "../../src/engine/light.cpp"
#include <engine\shadow.hpp>
#include <engine\assets.hpp>
#include <engine\sceneGraph.hpp>
#include "../../src/engine/gameObject.cpp"

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
glm::vec3 lightPos(1.2f, 3.0f, 2.0f);

const uint32_t k_shadow_height = 1024;
const uint32_t k_shadow_width = 1024;
const float k_shadow_near = 1.0f;
const float k_shadow_far = 7.5f;

float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
    Input* input = Input::instance();

    if (input->isKeyPressed(GLFW_KEY_W)) {
        camera.handleKeyboard(Camera::Movement::Forward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_S)) {
        camera.handleKeyboard(Camera::Movement::Backward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_A)) {
        camera.handleKeyboard(Camera::Movement::Left, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_D)) {
        camera.handleKeyboard(Camera::Movement::Right, dt);
    }
}

void onKeyPress(int key, int action) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(true);
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(false);
    }
}

void onMouseMoved(float x, float y) {
    if (firstMouse) {
        firstMouse = false;
        lastX = x;
        lastY = y;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    camera.handleMouseScroll(y);
}

std::pair<uint32_t, uint32_t> createFBO() {
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    uint32_t depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, k_shadow_width, k_shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer Incomplete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return std::make_pair(fbo, depthMap);
}

//void renderScene(const Shader& shader, const Geometry& quad, const Geometry& cube,
//    const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal, bool secondpass) {
//    if (secondpass) {
//        t_albedo.use(shader, "material.diffuse", 0);
//        t_specular.use(shader, "material.specular", 1);
//        t_normal.use(shader, "material.normal", 2);
//    }
//    
//
//    glm::mat4 model = glm::mat4(1.0);
//    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
//    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
//    shader.set("model", model);
//    if (secondpass) {
//        glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
//        shader.set("normalMat", normalMat);
//    }
//    
//    quad.render();
//
//    //model = glm::mat4(1.0);
//    //shader.set("model", model);
//    //if (secondpass) {
//    //    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
//    //    shader.set("normalMat", normalMat);
//    //}
//    //cube.render();
//
//    model = glm::mat4(1.0);
//    model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
//    shader.set("model", model);
//    if (secondpass) {
//        glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
//        shader.set("normalMat", normalMat);
//    }
//    cube.render();
//}

void renderScene(Node& node1, Node& node2,const Shader& shader, const Geometry& quad, const Geometry& cube,
    const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal, bool secondpass) {
    if (secondpass) {
        t_albedo.use(shader, "material.diffuse", 0);
        t_specular.use(shader, "material.specular", 1);
        t_normal.use(shader, "material.normal", 2);
    }


    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    //shader.set("model", model);
   // if (secondpass) {
        //glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        //shader.set("normalMat", normalMat);
    //}

    //quad.render();
    node1.DrawGeometry(model, shader, quad, true);

    //model = glm::mat4(1.0);
    //shader.set("model", model);
    //if (secondpass) {
    //    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    //    shader.set("normalMat", normalMat);
    //}
    //cube.render();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
    //shader.set("model", model);
    //if (secondpass) {
   //     glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        //shader.set("normalMat", normalMat);
   // }
    //cube.render();

    node2.DrawGeometry(model, shader, cube, true);


}


void render(Node& node, Node& node2, const Geometry& quad, const Geometry& cube,
    const Shader& s_phong, const Shader& s_depth, const Shader& s_debug, const Shader& s_light,
    const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {

    //FIRST PASS
   /* glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, k_shadow_width, k_shadow_height);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);
    const glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    s_depth.use();
    s_depth.set("lightSpaceMatrix", lightSpaceMatrix);*/

    node.getMaterial().getShadow().FirstPass();

    //glCullFace(GL_FRONT);
    renderScene(node,node2,s_depth, quad, cube, t_albedo, t_specular, t_normal, false);
    //glCullFace(GL_BACK);

//SECOND PASS
    //shadow.unBindFrameBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glm::mat4 view = camera.getViewMatrix();
    //glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

    glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);
    /*
    s_phong.use();

    s_phong.set("view", view);
    s_phong.set("proj", proj);

    s_phong.set("viewPos", camera.getPosition());

    s_phong.set("dirLight.direction", lightPos);
    s_phong.set("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("dirLight.specular", 1.0f, 1.0f, 1.0f);

    s_phong.set("material.shininess", 32);
    */
    node.getMaterial().setMaterialProperties(camera.getPosition(),view,proj);

    /*
    s_phong.set("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    s_phong.set("depthMap", 3);
    */

    node.getMaterial().getShadow().setDepthMap(node.getMaterial().getShadow()._depth);

    renderScene(node, node2,s_phong, quad, cube, t_albedo, t_specular, t_normal, true);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
    //glClear(GL_COLOR_BUFFER_BIT);
    //glDisable(GL_DEPTH_TEST);

    //s_debug.use();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, fbo_texture);
    //s_debug.set("depthMap", 0);

    //quad.render();
}

void render(SceneGraph& sceneGraph, GameObject& floor, GameObject& cubeGo) {
    floor.Init();
    floor.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
    floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    floor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));
    floor.readyToDraw();

    cubeGo.Init();
    cubeGo.Translate(glm::vec3(0.0f, 2.0f, 0.0f));
    cubeGo.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    cubeGo.Scale(glm::vec3(1.0f, 1.0f, 1.0f));
    cubeGo.readyToDraw();

    sceneGraph.updateNodes(camera);



}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_phong("../projects/TEST/normalTest.vs", "../projects/TEST/normalTest.fs");
    const Shader s_depth("../projects/AG14/depth.vs", "../projects/AG14/depth.fs");
    const Shader s_debug("../projects/AG14/debug.vs", "../projects/AG14/debug.fs");
    const Shader s_light("../projects/AG14/light.vs", "../projects/AG14/light.fs");
    const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Texture t_normal("../assets/textures/bricks_normal.png", Texture::Format::RGB);
    const Quad quad(2.0f);
    const Quad quadTest(2.0f);
    const Cube cube(1.0f);
    //const Sphere sphere(1.0f, 25, 25);

    //auto fbo = createFBO();

    //Lights
    DirLight dirLight(glm::vec3(1.2f, 3.0f, 2.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
    //DirLight dirLight;

    std::vector<PointLight> pointLights = {
        { glm::vec3(20.0f, 2.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
        //{ glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
        //{ glm::vec3(-3.0f, 2.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) }
    };
    std::vector<SpotLight> spotLights = {
        { glm::vec3(20.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
        //{ glm::vec3(1.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
        //{ glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) }
    };


    Shadow shadow(s_depth, s_debug, quadTest, dirLight.getDirection());

    //Load Assets
    Assets assets;
    int assetFloor = assets.addNewGeometry(quad, t_albedo, t_specular, t_normal);
    int assetShip = assets.addNewGeometry(cube, t_albedo, t_specular, t_normal);  //assets.addNewModel(object); //assets.addNewGeometry(cube, t_albedo, t_specular, t_normal); 


    Material mainMaterial(s_phong, shadow, dirLight, spotLights, pointLights);

    SceneGraph sceneGraph(assets);

    GameObject floor(sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry));
    GameObject cubeGO(sceneGraph, Node(assetShip, mainMaterial, Node::Type::isGeometry));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    Node n0 = sceneGraph.getNode(0);
    Node n1 = sceneGraph.getNode(1);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);
        render(sceneGraph, floor, cubeGO);
       //render(n0, n1, quad, cube, s_phong, s_depth, s_debug, s_light, t_albedo, t_specular, t_normal);
        window->frame();
    }

    //glDeleteFramebuffers(1, &fbo.first);
    //glDeleteTextures(1, &fbo.second);

    return 0;
}