
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

#include "Camera.h"

using namespace glm;
Camera::Camera() {
	cameraPosition = vec3(0.0f, 0.0f, 0.0f);
	cameraTargetPos = vec3(0.0f, 0.0f, 0.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	cameraYaw = 0.0f;
	cameraPitch = 0.0f;
	cameraRadius = 10.0f;

	this->window = window;

}

mat4 Camera::getViewMatrix() const{
	return lookAt(cameraPosition, cameraTargetPos, cameraUp);
}



void Camera::setLookAt(const vec3& target) {
	cameraTargetPos = target;
}

void Camera::setRadius(float radius) {
	cameraRadius = clamp(radius, 0.0f, 500.0f);
}

void Camera::rotate(float yaw, float pitch) {
	cameraYaw = radians(yaw);
	cameraPitch = radians(pitch);

	cameraPitch = clamp(cameraPitch, -pi<float>() / 2.0f + 0.1f, pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	cameraPosition.x = cameraTargetPos.x + cameraRadius * cosf(cameraPitch) * sinf(cameraYaw);
	cameraPosition.y = cameraTargetPos.y + cameraRadius * sinf(cameraPitch);
	cameraPosition.z = cameraTargetPos.z + cameraRadius * cosf(cameraPitch) * cosf(cameraYaw);
}

glm::vec3 Camera::getCamPos() {
	return cameraPosition;
}
glm::vec3 Camera::getTargetPos() {
	return targetPosition;
}

float Camera::getYaw() {
	return degrees(cameraYaw);
}

float Camera::getViewAngle() {
	return viewAngle;
}
