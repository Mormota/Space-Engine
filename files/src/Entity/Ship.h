#pragma once

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <random>

#include "Entity.h"
#include "../Content/Resource.h"

#ifndef SHIP_H
#define SHIP_H

enum movementType {
	MOVING,
	IDLEING,
	DOCKED
};

struct Cargo {
	float mass;
	Resource resource;
};

class Ship : public Entity
{
public:
	Ship(Mesh model, ShaderProgram shader, int id = 0);

	void Init();

	void setTarget(glm::vec3 target);
	void setCurveAnchor(int anchorPointer, glm::vec3 anchorPosition);

	

	void updatePosition(double deltaTime);


	void templateForces();

	void setForces(float forceAcc, float forceDecc);
	void setMass(float mass);
	void setMaxSpeed(float speed);
	void setAttribs(float forceAcc, float forceDecc, float mass, float speed);

private:

	std::string shipName;


	movementType currentMovement;

	void idleMovement();
	void targetMovement(double deltaTime);
	void targetRotate(glm::vec3 offset);
	void setSpaceRotation(glm::vec3 position);
	void setSpacePosition(glm::vec3 position);
	bool isSlowing(float distance);
	void calculateAcc();
	void calculateSpeed(float distance, double deltaTime);

	


	float emptyMass;
	float forceAcc, forceDecc;

	float acc, decc;

	float currentSpeed;
	float maxSpeed;

	float warpSpeed = 5;

	bool warping = false;
	bool onTarget = false;

	float turnSpeed = 0.1;

	float angleDifference = 0;

	int idleAngle = 0;

	std::vector<glm::vec3> anchors = {
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0)
	};
	glm::vec3 targetPosition;

	
	glm::vec3 spacePosition;
	glm::vec3 spaceRotation;
	glm::vec3 idlePosition;
	float maxOffset = 0.1;
	float maxHeightDifference = 0.05;

	glm::vec3 idleSpeed = glm::vec3(0, 0, 0);
	glm::vec3 idleDirection = glm::vec3(1, -1, 1);

	
	
	
	Cargo currentCargo;
	float maxCargoMass = 100;

	double p1(double x);
	double p2(double x);
	double p3(double x);
	double p4(double x);
	double curvetate(double x);
};

#endif // !SHIP_H
