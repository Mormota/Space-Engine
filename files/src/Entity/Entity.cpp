#include "Entity.h"
#include <math.h> 

#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../SoundSystem/SoundSystem.h"


Entity::Entity(const std::string& Model, const std::string& texturePath, bool generateMipMaps, ShaderProgram shaderProgram, int id) {
	mesh.loadOBJ(Model);
	texture.loadTexture(texturePath);
	shader = shaderProgram;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;

	this->initData();
}

Entity::Entity(Mesh model, ShaderProgram shaderProgram, int id) {
	mesh = model;
	shader = shaderProgram;

	this->setID(id);

	this->initData();

}


Entity::Entity(Mesh model, Texture2D texture, ShaderProgram shaderProgram, int id) {
	mesh = model;
	shader = shaderProgram;

	this->texture = texture;
	this->setID(id);

	this->initData();
	
}

void Entity::initData() {
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotY = 0;
	rotX = 0;
	rotZ = 0;
}

void Entity::setScale(float scale) {
	Scale = glm::vec3(scale, scale, scale);
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
	this->id = id;
	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
}

int Entity::getID() {
	return id;
}

Entity::~Entity() {}

glm::mat4 Entity::getModelMatrix() {

	return glm::translate(glm::mat4(), Position) *
		glm::scale(glm::mat4(), Scale * 2.5f) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Entity::render() {
	
	
	shader.setUniform("model", this->getModelMatrix());

	shader.setUniform("pickingColor", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	shader.setUniform("pickingVector", glm::vec2(0.0f, 0.0f));
	shader.setUniform("materialVector", glm::vec2(1.0f, 0.0f));

	mesh.draw();
}

void Entity::textureRender() {
	modelMatrix = glm::translate(glm::mat4(), Position) *
		glm::scale(glm::mat4(), Scale * 2.5f) *
		glm::rotate(glm::mat4(), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setUniform("model", modelMatrix);

	shader.setUniform("pickingColor", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	shader.setUniform("pickingVector", glm::vec2(0.0f, 0.0f));
	shader.setUniform("materialVector", glm::vec2(0.0f, 0.0f));

	texture.bind(0);
	mesh.draw();
	texture.unBind();
}

void Entity::pickingRender() {

	//std::cout << Position.x << " position " << std::endl;

	shader.setUniform("model", this->getModelMatrix());
	shader.setUniform("pickingColor", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	shader.setUniform("pickingVector", glm::vec2(1.0f, 0.0f));

	mesh.draw();
}



glm::vec3 Entity::getId() {
	return glm::vec3(r, g, b);
}

Mesh Entity::getMesh() {
	return mesh;
}

Texture2D Entity::getTexture() {
	return texture;
}

void Entity::setType(entityTypes type) {
	entityType = type;
}

void Entity::addSound(const char* filename) {
	sound.loadSound(filename);
	sound.setListenerPos(cameraPos);
	sound.setLooping(true);
	sound.setSoundPos(Position);
	//sound.play();
}

void Entity::playSound() {
	sound.play();
}

void Entity::getCamera(Camera camera) {
	sound.setSoundPos(Position);
	sound.setListenerPos(camera.getCamPos());
	sound.setVolume(1.2);
}


