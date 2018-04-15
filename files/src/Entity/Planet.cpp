#include "Planet.h"
#include "Entity.h"

Planet::Planet(Mesh model, Texture2D texture, ShaderProgram shader, int id) :Entity(model, texture, shader, id) {

}

void Planet::rotate(double deltaTime) {
	int rotY = this->getRotation().y + 0.1f;
	rotation += rotationSpeed * deltaTime;
	this->setRotation(0, rotation, 0);
}

void Planet::orbit(double deltaTime) {
	float posY = orbitCenter.y;
	float posZ = (sinf(radians((float)orbitRotation)) * distanceFromCenter) + orbitCenter.z;
	float posX = (cosf(radians((float)orbitRotation)) * distanceFromCenter) + orbitCenter.z;

	float oval = sinf(radians(orbitRotation));

	orbitRotation = orbitRotation + (orbitalSpeed * (float)deltaTime);

	this->setPosition(glm::vec3(posX, posY, posZ + oval * distortion));
}

void Planet::setPlanetAttributes(
	const string name,
	float planetRotation,
	float orbitalRotation,
	float planetRotationSpeed,
	float orbitalRotationSpeed,
	glm::vec3 orbitalCenter,
	float distanceFromCenter,
	float distortion) 
{
	this->setName(name);
	this->setPlanetRotation(planetRotation);
	this->setOrbitalRotation(orbitalRotation);
	this->setRotationSpeed(planetRotationSpeed);
	this->setOrbitalRotationSpeed(orbitalRotationSpeed);
	this->setOrbitalCenter(orbitalCenter);
	this->setDistanceFromCenter(distanceFromCenter);
	this->setDistortion(distortion);
}
void Planet::setName(const string name){
	this->name = name;
}
void Planet::setPlanetRotation(float rotation) {
	this->rotation = rotation;
}
void Planet::setOrbitalRotation(float orbitalRotation) {
	this->orbitRotation = orbitalRotation;
}
void Planet::setOrbitalCenter(glm::vec3 centerPosition) {
	this->orbitCenter = centerPosition;
}
void Planet::setDistanceFromCenter(float distanceFromCenter) {
	this->distanceFromCenter = distanceFromCenter;
}
void Planet::setRotationSpeed(float rotationSpeed) {
	this->rotationSpeed = rotationSpeed;
}
void Planet::setOrbitalRotationSpeed(float orbitalRotationSpeed) {
	this->orbitalSpeed = orbitalRotationSpeed;
}
void Planet::setDistortion(float distortion) {
	this->distortion = distortion;
}
