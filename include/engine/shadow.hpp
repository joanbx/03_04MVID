#ifndef __SHADOW_H__
#define __SHADOW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/shader.hpp>
#include <engine/light.hpp>

class Shadow {

	const uint32_t k_shadow_height = 1024;
	const uint32_t k_shadow_width = 1024;
	const float k_shadow_near = 1.0f;
	const float k_shadow_far = 7.5f;


private:

	glm::mat4 lightSpaceMatrix;
	DirLight& _dirLight;
	uint32_t _fbo;
	uint32_t _depth_map;

public:

	const Shader& _depth;

	Shadow(const Shader& depth, DirLight& dirLight);

	Shadow() = delete;
	~Shadow();

	Shadow(const Shadow&) = default;
	Shadow(Shadow&&) = default;
	Shadow& operator=(const Shadow&) = default;
	Shadow& operator=(Shadow&&) = default;

	void FirstPass();
	void createFBO();
	void setDepthMap(const Shader& shader);

};

#endif