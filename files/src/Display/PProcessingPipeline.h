#pragma once
#include <stdio.h>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW\glfw3.h"

#include "../Camera/Camera.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"

class PostProcessing {
public:
	PostProcessing(ShaderProgram shader, int width, int height);
	void init();
	void render(int width, int height);

private:
	int width, height;

	Mesh mesh;
	Texture2D texture;
	ShaderProgram shader;

	GLuint renderedTexture;
	GLuint frameBuffer = 0;
	GLuint depthrenderbuffer;

};