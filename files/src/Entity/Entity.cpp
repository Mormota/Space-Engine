#include "Entity.h"

#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"


Entity::Entity(const std::string& Model, const std::string& texturePath, bool generateMipMaps, ShaderProgram shaderProgram, int id) {
	mesh.loadOBJ(Model);
	texture.loadTexture(texturePath);
	shader = shaderProgram;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;

	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotY = 0;
	rotX = 180;
	rotZ = 0;
}

void Entity::setPickingShader(ShaderProgram pickingShader) {
	this->pickingShader = pickingShader;
}

glm::vec3 Entity::getRotation() {
	return glm::vec3(rotX, rotY, rotZ);
}

glm::vec3 Entity::getPosition() {
	return Position;
}

void Entity::setPosition(glm::vec3 newPosition) {
	Position = newPosition;
}

void Entity::setRotation(float rotX, float rotY, float rotZ) {
	this->rotX = rotX;
	this->rotY = rotY;
	this->rotZ = rotZ;
}

void Entity::setID(int id) {
	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
}


Entity::~Entity() {}

void Entity::render() {
	modelMatrix = glm::translate(glm::mat4(), Position) * 
		glm::scale(glm::mat4(), Scale) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	
	shader.setUniform("model", modelMatrix);

	shader.setUniform("pickingColor", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	shader.setUniform("pickingVector", glm::vec2(0.0f, 0.0f));

	texture.bind(0);
	mesh.draw();
	texture.unBind();
}

void Entity::pickingRender() {
	modelMatrix = glm::translate(glm::mat4(), Position) *
		glm::scale(glm::mat4(), Scale) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setUniform("model", modelMatrix);
	shader.setUniform("pickingColor", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	shader.setUniform("pickingVector", glm::vec2(1.0f, 0.0f));

	mesh.draw();
}