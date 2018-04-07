#include "Gui.h"
#include <iostream>

#include "GL\glew.h"
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"


void mouseButtonCallbackGUI(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//return true;
	}
}


Gui::Gui(const std::string& texturePath, ShaderProgram shader){
	texture.loadTexture(texturePath);
	this->guiShader = shader;
	guiModel.loadOBJ("gui.obj");

	Position = glm::vec2(0.0f, 0.0f);
	Scale = glm::vec2(0.08f, 0.24f);
}


Gui::~Gui(){}

void Gui::setDisplay(int displayWidth, int displayHeight) {
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;
}

void Gui::scaleInPixels(int width, int height, int displayWidth, int displayHeight) {
	this->setDisplay(displayWidth, displayHeight);
	this->scaleInPixels(width, height);
}

void Gui::scaleInPixels(int width, int height) {

	this->width = width;
	this->height = height;

	relativeWidth = (float)(double)width / (double)displayWidth;
	relativeHeight = (float)(double)height / (double)displayHeight;

	Scale = glm::vec2(relativeWidth, relativeHeight);
}

void Gui::positionInPixels(int dx, int dy, int displayWidth, int displayHeight) {
	this->setDisplay(displayWidth, displayHeight);
	this->positionInPixels(dx, dy);
}

void Gui::positionInPixels(int dx, int dy) {

	this->dx = dx;
	this->dy = dy;

	float relativeYOffset = (float)((double)dy / (double)displayHeight) + 1.0f - relativeHeight;
	float relativeXOffset = (float)((double)dx / (double)displayWidth) - 1.0f + relativeWidth;

	Position.y = relativeYOffset;
	Position.x = relativeXOffset;
}


bool Gui::onClick(GLFWwindow* window) {
	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	static int oldState = 0;
	int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (newState == 0 && oldState == 1) {
		if (
			((dx < posX) && (posX < (dx + width))) &&
			((dy < posY) && (posY < (dy + height)))) {
			oldState = newState;
			return true;
		}
	}
	oldState = newState;
	

	return false;
}

void Gui::render() {
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(Position, 1.0f)) *
		glm::scale(glm::mat4(), glm::vec3(Scale, 1.0f)) *
		glm::rotate(glm::mat4(), glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));

	guiShader.use();
	guiShader.setUniform("model", modelMatrix);



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	texture.bind();
	guiModel.draw();
	texture.unBind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	guiShader.stop();
}

bool Gui::isClicked(GLFWwindow* window) {
	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	if (
		((dx < posX) && (posX < (dx + width))) &&
		((dy < posY) && (posY < (dy + height)))) {
		return true;
	}
	else return false;
}

