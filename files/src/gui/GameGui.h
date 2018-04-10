#pragma once

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "../Model/Mesh.h"
#include "../ShaderProgram/ShaderProgram.h"

#include "../Model/Texture2D.h"

#ifndef GAMEGUI_H
#define GAMEGUI_H

class GameGui
{
public:
	GameGui();
	~GameGui();
private:
	GLFWwindow* window;

};


#endif // !GAMEGUI_H


