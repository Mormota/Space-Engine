#include "glm\glm.hpp"

using namespace glm;
#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:

	Camera();

	mat4 getViewMatrix() const;

	virtual void rotate(float yaw, float pitch);

	void setLookAt(const vec3& target);
	void setRadius(float radius);

	glm::vec3 getCamPos();


private:
	
	GLFWwindow* window;

	glm::vec3 cameraPosition;
	vec3 cameraTargetPos;
	vec3 cameraUp;

	void updateCameraVectors();
	float cameraRadius;

	//angles [RADIANS]
	float cameraYaw, cameraPitch;

	glm::vec3 targetPosition = glm::vec3(0, 0, 0);
};

#endif // !CAMERA_H
