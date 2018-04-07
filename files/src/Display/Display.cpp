#include <stdio.h>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

#include "Display.h"



bool fullScreen = false;

Display::Display(int windowWidth, int windowHeight, const char* title) {
	width = windowWidth;
	height = windowHeight;
	windowTitle = title;
}

GLFWwindow* Display::initOpenGl() {
	if (!initGlfw()) return false;
	pWindow = initWindow(pWindow);
	if (!initGlew()) return false;

	glEnable(GL_DEPTH_TEST);
	return pWindow;
}

void Display::getFrames() {
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	//limit text
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		fpsCount = fps;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< windowTitle << "  "
			<< "FPS: " << fps << "  "
			<< "FrameTime: " << msPerFrame << "ms";
		glfwSetWindowTitle(pWindow, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;
}



bool Display::initGlfw() {
	if (!glfwInit()) {
		std::cerr << "GLFW could not be initialized" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	return true;
}

GLFWwindow* Display::initWindow(GLFWwindow* window) {
	if (!fullScreen) {
		window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	}
	else {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
		if (videoMode != NULL) window = glfwCreateWindow(videoMode->width, videoMode->height, windowTitle, monitor, NULL);
		else {
			window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
			std::cerr << "Could not create fullscreen window" << std::endl;
		}
	}

	if (window == NULL) {
		std::cerr << "Window could not be created" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, keyboardCallback);

	return window;
}

bool Display::initGlew() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW vould not be initialized" << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}

void Display::update() {
	//std::cout << "Current Fps rate: " << fpsCount << std::endl;
	glfwSwapBuffers(pWindow);
}

void Display::initDisplay() {
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Display::useShader(ShaderProgram shader, Camera camera) {
	glm::mat4 viewMatrix, projectionMatrix;

	viewMatrix = camera.getViewMatrix();

	projectionMatrix = glm::perspective(radians(45.0f), (float)width / height, 0.1f, 1000.0f);

	shader.setUniform("view", viewMatrix);
	shader.setUniform("projection", projectionMatrix);
	glUseProgram(0);
	shader.use();
}


void Display::cleanUpPickingShader() {
	pickingShader.cleanUp();
}