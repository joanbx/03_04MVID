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
//Class Shadow: Paints directional shadow. Needs depth and debug shaders, plus a quad geometry and a vec3 direction (dirLight)
class Shadow {
	//Constants
	const uint32_t k_shadow_height = 1024;
	const uint32_t k_shadow_width = 1024;
	const float k_shadow_near = 1.0f;
	const float k_shadow_far = 7.5f;


private:

	glm::mat4 lightSpaceMatrix;
	glm::vec3& _direction;
	std::pair<uint32_t, uint32_t> fbo;
	const Geometry& _quadDebug;
public:
	//Shaders
	const Shader& _depth;
	const Shader& _sDebug;
	//Constructor
	Shadow(const Shader& depth, const Shader& sDebug, const Geometry& quadDebug, glm::vec3& direction);
	Shadow() = delete;
	~Shadow();

	//FirstPass: First pass of the shadow
	void FirstPass();
	//createFBO: Creats frame buffer needed to create the shadow
	std::pair<uint32_t, uint32_t>  createFBO();
	//setDepthMap: sets depth map to custom shader
	void setDepthMap(const Shader& shader);
	//setTestDepth
	void setTestDepth();
	//unBindFrameBuffer
	void unBindFrameBuffer();
};

#endif