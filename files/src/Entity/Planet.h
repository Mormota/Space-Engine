#pragma once

#include "glm\glm.hpp"
#include "glm/gtx/transform.hpp"

#include "GL\glew.h"
#include <iostream>
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "Entity.h"

#ifndef PLANET_H
#define PLANET_H

#include <string>

class Planet : public Entity{
public:
	Planet(Mesh model, Texture2D texture, ShaderProgram shader, int id);

	//movement
	void rotate(double deltaTime);
	void orbit(double deltaTime);

	//setters
	void setPlanetAttributes(
		const string name, 
		float planetRotation, 
		float orbitalRotation,
		float planetRotationSpeed, 
		float orbitalRotationSpeed, 
		glm::vec3 orbitalCenter,
		float distanceFromCenter,
		float distortion
		);
	void setName(const string name);
	void setPlanetRotation(float rotation);
	void setOrbitalRotation(float orbitalRotation);
	void setOrbitalCenter(glm::vec3 centerPosition);
	void setDistanceFromCenter(float distanceFromCenter);
	void setRotationSpeed(float rotationSpeed);
	void setOrbitalRotationSpeed(float orbitalRotationSpeed);
	void setDistortion(float distortion);

private:
	string name = "Unnamed";
	float rotation = 0;
	float orbitRotation = 0;
	glm::vec3 orbitCenter = glm::vec3(0, 0, 0);
	float distanceFromCenter = 25;

	int rotationSpeed = 20;
	int orbitalSpeed = 36;

	float distortion = 0;


};

#endif // !PLANET_H

