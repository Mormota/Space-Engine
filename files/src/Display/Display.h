#pragma once
#include <stdio.h>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW\glfw3.h"

#include "../Camera/Camera.h"
#include "../ShaderProgram/ShaderProgram.h"


#ifndef Initialize_h
#define Initialize_h

class Display {
public:
	Display(int width, int height,const char* title);
	GLFWwindow * initOpenGl();
	void getFrames();
	void initDisplay();

	void useShader(ShaderProgram shader, Camera camera);
	void usePickingShader(ShaderProgram shader, Camera camera);
	void cleanUpPickingShader();

	void update();
	bool mouseLeftPressed();
	bool mouseRightPressed();
	bool mouseMiddlePressed();

	void getEntityId(int& id);

	double getDeltaTime();
	void setDeltaTime();
private:

	ShaderProgram pickingShader;
	
	GLFWwindow* initWindow(GLFWwindow* window);
	bool initGlew();
	bool initGlfw();
	void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
	

	const char* windowTitle;
	int width;
	int height;
	GLFWwindow* pWindow = NULL;

	double fpsCount;
	double deltaTime;

	int oldLeftState = 0, oldRightState = 0, oldMiddleState = 0;
	double lastFrame = 0;


};
#endif // !Initialize.h