#include <engine/shadow.hpp>
#include <iostream>



Shadow::Shadow(const Shader& depth, const Shader& sDebug, const Geometry& quadDebug, glm::vec3& direction) : _depth(depth), _sDebug(sDebug), _quadDebug(quadDebug), _direction(direction) {
	fbo = createFBO();
	//uint32_t _fbo;
	//uint32_t _depth_map;
}

Shadow::~Shadow() {
	glDeleteFramebuffers(1, &fbo.first);
	glDeleteTextures(1, &fbo.second);
}

std::pair<uint32_t, uint32_t>  Shadow::createFBO() {
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

void Shadow::FirstPass() {
	

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.first);
	glViewport(0, 0, k_shadow_width, k_shadow_height);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);
	glm::mat4 lightView = glm::lookAt(_direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpaceMatrix = lightProjection * lightView;

	_depth.use();
	_depth.set("lightSpaceMatrix", lightSpaceMatrix);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void Shadow::unBindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Shadow::setDepthMap(const Shader& shader) {
	//SECOND PASS

	shader.set("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, fbo.second);
	shader.set("depthMap", 3);
	
}

void Shadow::setTestDepth() {
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	_sDebug.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.second);
	_sDebug.set("depthMap", 0);

	_quadDebug.render();
	
}
