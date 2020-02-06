#ifndef __SHADOW_H__
#define __SHADOW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/shader.hpp>
#include <engine/light.hpp>

#include <engine\window.hpp>
#include <engine\geometry\geometry.hpp>

class Shadow {

	const uint32_t k_shadow_height = 1024;
	const uint32_t k_shadow_width = 1024;
	const float k_shadow_near = 1.0f;
	const float k_shadow_far = 7.5f;


private:

	glm::mat4 lightSpaceMatrix;
	glm::vec3& _direction;
	//uint32_t fbo;
	//uint32_t depthMap;
	std::pair<uint32_t, uint32_t> fbo;
	const Geometry& _quadDebug;
public:

	const Shader& _depth;
	const Shader& _sDebug;

	Shadow(const Shader& depth, const Shader& sDebug, const Geometry& quadDebug, glm::vec3& direction);

	Shadow() = delete;
	~Shadow();

	//Shadow(const Shadow&) = default;
	//Shadow(Shadow&&) = default;
	//Shadow& operator=(const Shadow&) = default;
	//Shadow& operator=(Shadow&&) = default;

	void FirstPass();
	std::pair<uint32_t, uint32_t>  createFBO();
	void setDepthMap(const Shader& shader);
	void setTestDepth();
	void unBindFrameBuffer();
};

#endif