#include <engine/shadow.hpp>
#include <iostream>

Shadow::Shadow(const Shader& depth, DirLight& dirLight) : _depth(depth), _dirLight(dirLight) {
	createFBO();
}

Shadow::~Shadow() {
	glDeleteFramebuffers(1, &_fbo);
	glDeleteTextures(1, &_depth_map);
}

void Shadow::FirstPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, k_shadow_width, k_shadow_height);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);
	const glm::mat4 lightView = glm::lookAt(_dirLight.getDirection(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	lightSpaceMatrix = lightProjection * lightView;

	_depth.use();
	_depth.set("lightSpaceMatrix", lightSpaceMatrix);
	
	//glCullFace(GL_FRONT);
	//--> RENDER EACH!!
	//glCullFace(GL_BACK);

}

void Shadow::createFBO() {
	//uint32_t fbo;
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	//const uint32_t k_shadow_height = 1024;
	//const uint32_t k_shadow_width = 1024;
	//uint32_t depthMap;
	glGenTextures(1, &_depth_map);
	glBindTexture(GL_TEXTURE_2D, _depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, k_shadow_width, k_shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Incomplete" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//TRY WITH RETURN DUPLE

}

void Shadow::setDepthMap(const Shader& shader) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _depth_map);
	shader.set("lightSpaceMatrix", 2);
	shader.set("depthMap", 2);
}
