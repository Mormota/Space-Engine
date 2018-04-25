#pragma once

#include <vector>
#include <string>

#include "GL\glew.h"
#include "glm\glm.hpp"

#ifndef MESH_H
#define MESH_H

struct Material {
	std::string name;
	glm::vec3 color;
};

struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 UV;
	glm::vec3 color;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadOBJ(const std::string& fileName);
	bool loadMTL(const std::string& fileName);
	bool loadMatObj(const std::string& fileName);
	void draw();
	void cleanUp();

	bool getLoaded();

private:
	std::string filename;
	void initBuffers();
	void setFilename(std::string filename);
	void enableAttribs();

	bool isLoaded;
	bool isMTLloaded;

	std::vector<Vertex> Vertices;
	std::vector<Material> Materials;

	GLuint VBO, VAO;
};

#endif // !MESH_H

