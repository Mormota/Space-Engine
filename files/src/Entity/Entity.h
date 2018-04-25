#pragma once

#include "glm\glm.hpp"
#include "glm/gtx/transform.hpp"

#include "GL\glew.h"
#include <iostream>
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "../SoundSystem/SoundSystem.h"

#ifndef ENTITY_H
#define ENTITY_H

enum entityTypes {
	UNSET,
	SHIP,
	ENTITY_PLANET
};

class Entity{
public:
	Entity(const std::string& Model, const std::string& texturePath, bool generateMipMaps, ShaderProgram shader, int id = 1);
	Entity(Mesh model, Texture2D texture, ShaderProgram shader, int id = 1);
	Entity(Mesh model, ShaderProgram shaderProgram, int id = 0);

	virtual ~Entity();

	void setPickingShader(ShaderProgram pickingShader);
	
	void render();
	void pickingRender();
	void textureRender();

	glm::vec3 getId();

	glm::vec3 getRotation();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 newPosition);
	void setRotation(float rotX, float rotY, float rotZ);
	void setID(int ID);
	void setScale(float scale);
	void initData();
	int getID();

	Mesh getMesh();
	Texture2D getTexture();

	void setType(entityTypes type);

	void addSound(const char* Filename);
	void getCamera(Camera camera);
	void playSound();

	entityTypes entityType = UNSET;

private:



	Mesh mesh;
	Texture2D texture;
	ShaderProgram shader;
	ShaderProgram pickingShader;
	SoundSystem sound;

	glm::vec3 colorID;
	glm::mat4 modelMatrix;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	float rotX, rotY, rotZ;
	int r, g, b;

	int id;

	

	glm::vec3 cameraPos = glm::vec3(0, 0, 0);
};
#endif // !ENTITY_H

