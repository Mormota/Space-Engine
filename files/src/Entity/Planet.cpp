#include "Planet.h"
#include "Entity.h"

Planet::Planet(Mesh model, Texture2D texture, ShaderProgram shader, int id, vector<Planet> subOrbits) :Entity(model, texture, shader, id) {
	this->subOrbits = subOrbits;
}

Planet::Planet(Mesh model, ShaderProgram shader, int id, vector<Planet> subOrbits) : Entity(model, shader, id) {
	this->subOrbits = subOrbits;
}

void Planet::rotate(double deltaTime) {
	int rotY = this->getRotation().y + 0.1f;
	rotation += rotationSpeed * deltaTime;
	this->setRotation(0, rotation, 0);

	int currentSubOrbit = 0;
	while (currentSubOrbit < subOrbits.size()) {
		Planet orbit = subOrbits[currentSubOrbit];
		orbit.rotate(deltaTime);
		subOrbits[currentSubOrbit] = orbit;
		currentSubOrbit++;
	}
}

void Planet::orbit(double deltaTime) {
	float posY = orbitCenter.y;
	float posZ = (sinf(radians((float)orbitRotation)) * distanceFromCenter) + orbitCenter.z;
	float posX = (cosf(radians((float)orbitRotation)) * distanceFromCenter) + orbitCenter.x;

	float ovalZ = sinf(radians(orbitRotation + distortionAngle));
	float ovalX = sin(radians(orbitRotation + distortionAngle));

	orbitRotation = orbitRotation + (orbitalSpeed * deltaTime);

	float circulateOffset = sinf(radians(orbitAngle)) * sinf(radians(orbitRotation)) * distanceFromCenter;

	this->setPosition(glm::vec3(posX + offset.x + ovalX * distortion, posY + offset.y + circulateOffset, posZ + offset.z + ovalZ * distortion));
	glm::vec3 currentPos = this->getPosition();

	int currentSubOrbit = 0;
	while (currentSubOrbit < subOrbits.size()) {
		Planet orbit = subOrbits[currentSubOrbit];
		orbit.orbit(deltaTime);
		subOrbits[currentSubOrbit] = orbit;
		currentSubOrbit++;
	}

}

void Planet::setType(planetType type) {
	if (type == EARTH_PLANET) {
		this->setScale(1);
	}
	else if (type == MOON) {
		this->setScale(0.15);
	}
	else if (type == SUN) {
		this->setScale(4.5);
	}
}

void Planet::addSubOrbit(Planet orbit) {
	subOrbits.push_back(orbit);
}

void Planet::setDistortionAngle(float angle) {
	distortionAngle = angle;
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

void Planet::setOffset(glm::vec3 offset) {
	this->offset = offset;
}

void Planet::setOffsetX(float offset) {
	this->offset.x = offset;
}

void Planet::setOffsetY(float offset) {
	this->offset.y = offset;
}

void Planet::setOffsetZ(float offset) {
	this->offset.z = offset;
}


string Planet::getName() {
	return name;
}

void Planet::planetRender() {
	this->render();
	for (Planet subOrbit : subOrbits) {
		subOrbit.render();
	}
}

void Planet::planetTextureRender() {
	this->textureRender();
	for (Planet subOrbit : subOrbits) {
		subOrbit.textureRender();
	}
}

void Planet::planetPickingRender() {
	this->pickingRender();
	for (Planet subOrbit : subOrbits) {
		subOrbit.pickingRender();
	}
}



void Planet::setCirculateAngle(float angle) {
	orbitAngle = angle;
}
