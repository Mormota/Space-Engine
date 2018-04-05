#include "Entity.h"

#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"


Entity::Entity(const std::string& Model, const std::string& texturePath, bool generateMipMaps, ShaderProgram shaderProgram) {
	mesh.loadOBJ(Model);
	texture.loadTexture(texturePath);
	shader = shaderProgram;

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


Entity::~Entity() {}

void Entity::pickingRender() {
	modelMatrix = glm::translate(glm::mat4(), Position) *
		glm::scale(glm::mat4(), Scale) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

	//shader.setUniform("model", modelMatrix);

	pickingShader.setUniform("model", modelMatrix);

	pickingShader.setUniform("PickingColor", glm::vec4(0.0, 1.0, 1.0, 1.01));

	texture.bind(0);
	mesh.draw();
	texture.unBind();
}

void Entity::render() {
	modelMatrix = glm::translate(glm::mat4(), Position) * 
		glm::scale(glm::mat4(), Scale) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	
	shader.setUniform("model", modelMatrix);

	/*pickingShader.setUniform("model", modelMatrix);

	pickingShader.setUniform("PickingColor", glm::vec4(0.0, 1.0, 1.0, 1.01));*/

	texture.bind(0);
	mesh.draw();
	texture.unBind();
}