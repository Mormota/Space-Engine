#pragma once

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "../Model/Mesh.h"
#include "../ShaderProgram/ShaderProgram.h"

#include "../Model/Texture2D.h"

class Gui
{
public:
	Gui(const std::string& texturePath, ShaderProgram shader);
	~Gui();

	bool isClicked(GLFWwindow* window);

	void render();

	void setDisplay(int displayWidth, int displayHeight);

	void scaleInPixels(int width, int height, int displayWidth, int displayHeight);
	void scaleInPixels(int width, int height);

	void positionInPixels(int dx, int dy, int displayWidth, int displayHeight);
	void positionInPixels(int dx, int dy);

	bool onClick(GLFWwindow* window);
	bool pressed = false;
private:
	Mesh guiModel;
	ShaderProgram guiShader;
	Texture2D texture;

	glm::vec2 align;

	float relativeWidth, relativeHeight;
	int width, height;
	int dx, dy;

	int displayWidth, displayHeight;

	glm::mat4 modelMatrix;
	glm::vec2 Position;
	glm::vec2 Scale;
	float rot = 0;

};

