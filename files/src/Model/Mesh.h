#pragma once

#include <vector>
#include <string>

#include "GL\glew.h"
#include "glm\glm.hpp"

#ifndef MESH_H
#define MESH_H

struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 UV;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadOBJ(const std::string& fileName);
	void guiLoader();
	void draw();

private:
	void initBuffers();

	bool isLoaded;

	std::vector<Vertex> Vertices;

	GLuint VBO, VAO;
};

#endif // !MESH_H

