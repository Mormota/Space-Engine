#pragma once

#include "glm\glm.hpp"
#include "glm/gtx/transform.hpp"

#include "GL\glew.h"
#include <iostream>
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"

#ifndef ENTITY_H
#define ENTITY_H


class Entity
{
public:
	Entity(const std::string& Model, const std::string& texturePath, bool generateMipMaps, ShaderProgram shader);
	virtual ~Entity();

	void setPickingShader(ShaderProgram pickingShader);
	
	void render();

	//void setup();

	glm::vec3 getRotation();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 newPosition);
	void setRotation(float rotX, float rotY, float rotZ);
	void setID(int ID);


private:
	Mesh mesh;
	Texture2D texture;
	ShaderProgram shader;
	ShaderProgram pickingShader;

	glm::vec3 colorID;
	glm::mat4 modelMatrix;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	float rotX, rotY, rotZ;
};
#endif // !ENTITY_H

