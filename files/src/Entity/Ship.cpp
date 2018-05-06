#include "Ship.h"




float calculateDistance(glm::vec3 offset) {
	float hd = sqrt(offset.x*offset.x + offset.z*offset.z);
	float d = sqrt(hd*hd + offset.y*offset.y);
	return d;
}

glm::vec3 calculateAngles(glm::vec3 offset) {
	float angle = degrees(atan2(offset.x, offset.z));
	float offsetDistance = calculateDistance(offset);
	float pitch = degrees(asin(offset.y / offsetDistance));

	return glm::vec3(-pitch, angle, 0);
}


void Ship::templateForces() {
	forceAcc = 20;
	forceDecc = 20;
	emptyMass = 50;
	maxSpeed = 5;
	this->setTarget(glm::vec3(0, 0, 100));
	this->Init();
	glm::vec2 vec1 = normalize(glm::vec2(0, 2));
	glm::vec2 vec2 = normalize(glm::vec2(0, -2));
	glm::vec2 vec3 = vec2 - vec1;
	float differenceLength = glm::length(vec3);
	float offsetAngle = 0;
	std::cout << vec3.y << " - " << glm::length(vec3) << std::endl;
}

Ship::Ship(Mesh model, ShaderProgram shader, int id):Entity(model, shader, id){
	this->setSpacePosition(glm::vec3(0, 0, 0));
}

void Ship::Init() {
	this->calculateAcc();
}

void Ship::setTarget(glm::vec3 target) {
	targetPosition = target;
}

void Ship::setSpacePosition(glm::vec3 position) {
	spacePosition = position;
	this->setPosition(position);
}

void Ship::setSpaceRotation(glm::vec3 rotation) {
	spaceRotation = rotation;
	this->setRotation(rotation.x, rotation.y, rotation.z);
}

void Ship::setCurveAnchor(int anchorPointer, glm::vec3 anchorPosition) {
	anchors[anchorPointer] = anchorPosition;
}

void Ship::setForces(float forceAcc, float forceDecc) {
	this->forceAcc = forceAcc;
	this->forceDecc = forceDecc;
	this->Init();
}

void Ship::setMass(float mass) {
	this->emptyMass = mass;
	this->Init();
}

void Ship::setMaxSpeed(float speed) {
	this->maxSpeed = speed;
	this->Init();
}

void Ship::setAttribs(float forceAcc, float forceDecc, float mass, float speed) {
	this->forceAcc = forceAcc;
	this->forceDecc = forceDecc;
	this->emptyMass = mass;
	this->maxSpeed = speed;
	this->Init();
}

void Ship::updatePosition(double deltaTime) {
	glm::vec3 position = spacePosition;
	glm::vec3 relativePos = position - targetPosition;
	float distance = calculateDistance(relativePos);

	if (floorf(distance * 500) / 500 != 0) {
		this->calculateSpeed(distance, deltaTime);
		this->targetMovement(deltaTime);
	}
	else {
		this->idleMovement();
	}

}

void Ship::calculateAcc() {
	acc = forceAcc / (emptyMass + currentCargo.mass);
	decc = forceDecc / (emptyMass + currentCargo.mass);
}

bool Ship::isSlowing(float distance) {
	float deccDistance = ((currentSpeed / 2 ) * (currentSpeed) / decc);
	return (deccDistance <= distance);
}

void Ship::calculateSpeed(float distance, double deltaTime) {
	float deltaSpeed;
	if (this->isSlowing(distance)) {
		//Accelerating
		deltaSpeed = acc * deltaTime;
		if (currentSpeed < maxSpeed && currentSpeed + deltaSpeed <= maxSpeed) {
			currentSpeed += deltaSpeed;
		}
		else {
			currentSpeed = maxSpeed;
			warping = true;
		}
	}
	else {
		//Decelerating
		warping = false;
		deltaSpeed = decc * deltaTime;
		if (currentSpeed - deltaSpeed >= 0) {
			currentSpeed -= deltaSpeed;
		}
		else {
			currentSpeed = 0;
		}
	}
}





void Ship::targetMovement(double deltaTime) {
	if (currentMovement != MOVING) {
		currentMovement = MOVING;
	}
	glm::vec3 position = this->getPosition();
	glm::vec3 relativePos = glm::normalize(position - targetPosition);
	float distance = calculateDistance(relativePos);

	glm::vec3 offset = relativePos * currentSpeed * (float)deltaTime * abs((angleDifference / 180) - 1);
	if (warping && onTarget)
		offset *= warpSpeed;
	if (distance <= currentSpeed * (float)deltaTime) {
		this->setSpacePosition(targetPosition);
		return;
	}


	this->setSpacePosition(position - offset);
	this->targetRotate(offset);
	
}

void Ship::targetRotate(glm::vec3 offset) {
	glm::vec3 angles = calculateAngles(offset);
	glm::vec3 currentRotation = this->getRotation();

	glm::vec3 angleDifference = angles - currentRotation;
	this->angleDifference = abs(angleDifference.y);

	if (angleDifference.y < 0)
		currentRotation.y -= turnSpeed;
	else
		currentRotation.y += turnSpeed;
	if (abs(angleDifference.y) <= turnSpeed)
		currentRotation = angles;

	angles.y = currentRotation.y;

	onTarget = (angleDifference.y < 6);
	if (angles.x != angles.x)
		return;
	this->setSpaceRotation(angles);
}



void Ship::idleMovement() {
	if (currentMovement != IDLEING) {
		idleAngle = 0;
		currentMovement = IDLEING;
	}
	glm::vec3 position = this->getPosition();
	idlePosition = spacePosition;

	float offsetX = sinf(radians((float)idleAngle)) * maxOffset;

	float offsetY = sinf(radians((float)idleAngle * 3));
	idlePosition.y = spacePosition.y + offsetY * maxHeightDifference;

	float angleOffset = sinf(radians((float)idleAngle)) * 2;

	this->setPosition(idlePosition);
	this->setRotation(spaceRotation.x, spaceRotation.y + angleOffset, spaceRotation.z);
	idleAngle++;
	if (idleAngle == 100)
		targetPosition = glm::vec3(0, 0, 0);
}


double Ship::p1(double x) {
	double x1 = anchors[0].x;
	double y1 = anchors[0].y;

	double x2 = anchors[1].x;
	double y2 = anchors[1].y;

	double x3 = anchors[2].x;
	double y3 = anchors[2].y;

	double x4 = anchors[3].x;
	double y4 = anchors[3].y;

	return (x - x2) * (x - x3) * (x - x4);
}

double Ship::p2(double x) {
	double x1 = anchors[0].x;
	double y1 = anchors[0].y;

	double x2 = anchors[1].x;
	double y2 = anchors[1].y;

	double x3 = anchors[2].x;
	double y3 = anchors[2].y;

	double x4 = anchors[3].x;
	double y4 = anchors[3].y;

	return (x - x1) * (x - x3) * (x - x4);
}

double Ship::p3(double x) {
	double x1 = anchors[0].x;
	double y1 = anchors[0].y;

	double x2 = anchors[1].x;
	double y2 = anchors[1].y;

	double x3 = anchors[2].x;
	double y3 = anchors[2].y;

	double x4 = anchors[3].x;
	double y4 = anchors[3].y;
	return (x - x1) * (x - x2) * (x - x4);
}

double Ship::p4(double x) {
	double x1 = anchors[0].x;
	double y1 = anchors[0].y;

	double x2 = anchors[1].x;
	double y2 = anchors[1].y;

	double x3 = anchors[2].x;
	double y3 = anchors[2].y;

	double x4 = anchors[3].x;
	double y4 = anchors[3].y;
	return (x - x1) * (x - x2) * (x - x3);
}

double Ship::curvetate(double x) {
	double returnValue;

	double x1 = anchors[0].x;
	double y1 = anchors[0].y;

	double x2 = anchors[1].x;
	double y2 = anchors[1].y;

	double x3 = anchors[2].x;
	double y3 = anchors[2].y;

	double x4 = anchors[3].x;
	double y4 = anchors[3].y;
	if (y1 != y2 && y2 != y3 && y3 != y4)
		returnValue = (p1(x) * y1 / p1(x1)) + (p2(x) * y2 / p2(x2)) + (p3(x) * y3 / p3(x3)) + (p4(x) * y4 / p4(x4));
	else
		returnValue = x;
	return returnValue;
}





