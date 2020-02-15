#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glm/glm.hpp>

const float k_Yaw = -90.0f;
const float k_Pitch = 0.0f;
const float k_Speed = 2.5f;
const float k_Sensitivity = 0.1f;
const float k_FOV = 45.0f;
//Class Camera: Creates a (perspective) camera
class Camera {
    public:
        enum class Movement {
            Forward = 0,
            Backward = 1,
            Left = 2,
            Right = 3,
        };

        Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = k_Yaw, float pitch = k_Pitch);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		//getViewMatrix: Return mat4 view Matrix (LookAt function)
        glm::mat4 getViewMatrix() const;
		//getViewMatrixCustom: Return customized mat4 view Matrix
		glm::mat4 getViewMatrixCustom() const;
		//getFOV: Return Field of view
        float getFOV() const;
		//getPosition: vec3 position of camera
        glm::vec3 getPosition() const;
		//getFront: vec3 front of camera
		glm::vec3 getFront() const;
		//getProj: mat4 projection of camera
        glm::mat4 getProj() const;
		
		//setFPS: set fps mode (true/false)
        void setFPS(bool fps);
		//handleKeyboard: Handles keboard to change position
        void handleKeyboard(Movement direction, float dt);
		//handleMouseMovement: handles mouse to change rotation
        void handleMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		//handleMouseScroll: handle mouse scroll to change fov
        void handleMouseScroll(float yoffset);

    private:
        void updateCameraVectors();
    private:
        glm::vec3 _position, _front, _up, _right, _worldUp; //camera vectors
        float _yaw, _pitch; //camera yaw and pitch (rotation)
        float _fov; //Field of view
        float _near = 0.1f; //near. Todo: Customizable
        float _far = 100.0f; //far. Todo: Customizable
		bool _fps; //fps mode
};

#endif
