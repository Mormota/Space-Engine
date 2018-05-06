#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>


static std::vector<GLuint> vaos;
static std::vector<GLuint> vbos;

std::vector<std::string> split(std::string s, std::string t)
{
	std::vector<std::string> res;
	while (1)
	{
		int pos = s.find(t);
		if (pos == -1)
		{
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1, s.size() - pos - 1);
	}
	return res;
}


Mesh::Mesh():isLoaded(false){}

Mesh::~Mesh(){
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}


void Mesh::setFilename(std::string file) {
	this->filename = file;
}

bool Mesh::loadOBJ(const std::string& filename){
	setFilename(filename);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;


	if (filename.find(".obj") != std::string::npos){
		std::ifstream fin(filename, std::ios::in);
		if (!fin){
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}

		//std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)){
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;

			if (cmd == "v"){
				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;

				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt"){
				glm::vec2 uv;
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;

				tempUVs.push_back(uv);
			}
			else if (cmd == "vn"){
				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;
				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			else if (cmd == "f"){
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex;

				while (ss >> faceData){
					std::vector<std::string> data = split(faceData, "/");

					if (data[0].size() > 0){
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}

					if (data.size() >= 1){
						// Is face format v//vn?  If data[1] is empty string then
						// this vertex has no texture coordinate
						if (data[1].size() > 0)
						{
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}

					if (data.size() >= 2){
						// Does this vertex have a normal?
						if (data[2].size() > 0)
						{
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}
			}
		}

		// Close the file
		fin.close();


		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++){
			Vertex meshVertex;

			// Get the attributes using the indices

			if (tempVertices.size() > 0){
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}

			if (tempNormals.size() > 0){
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}

			if (tempUVs.size() > 0){
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.UV = uv;
			}
			meshVertex.specularColor = glm::vec3(1.0, 1.0, 1.0);
			Vertices.push_back(meshVertex);
		}

		// Create and initialize the buffers
		initBuffers();

		return (isLoaded = true);
	}

	// We shouldn't get here so return failure
	return false;
}

bool Mesh::loadMTL(const std::string& fileName) {
	std::vector<std::string> materialNames;
	std::vector<glm::vec3> materialColors;
	std::vector<glm::vec3> specularColors, ambientColors;
	if (fileName.find(".mtl") != std::string::npos) {
		std::ifstream fin(fileName, std::ios::in);
		if (!fin) {
			std::cerr << "Cannot open " << fileName << std::endl;
			return false;
		}


		std::string lineBuffer;
		std::cout << "Material file oppened: " << fileName << std::endl;
		while (std::getline(fin, lineBuffer)) {
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;
			if (cmd == "newmtl") {
				std::string materialName;
				ss >> materialName;
				materialNames.push_back(materialName);
			}
			else if(cmd == "Kd"){
				glm::vec3 colors;
				ss >> colors.x;
				ss >> colors.y;
				ss >> colors.z;
				materialColors.push_back(colors);
			}
			else if(cmd == "Ka"){
				glm::vec3 ambient;
				ss >> ambient.x;
				ss >> ambient.y;
				ss >> ambient.z;
				ambientColors.push_back(ambient);
			}
			else if (cmd == "Ks") {
				glm::vec3 specular;
				ss >> specular.x;
				ss >> specular.y;
				ss >> specular.z;
				specularColors.push_back(specular);
			}
		}

		// Close the file
		fin.close();
		for (int i = 0; i < materialNames.size(); i++) {
			Material material;
			material.name = materialNames[i];
			material.color = materialColors[i];
			material.ambientColor = ambientColors[i];
			material.specularColor = specularColors[i];
			Materials.push_back(material);
		}
	}
	return false;
}
bool Mesh::loadMatObj(const std::string& fileName) {
	setFilename(fileName);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices, materialIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	int materialId;


	if (fileName.find(".obj") != std::string::npos) {
		std::ifstream fin(fileName, std::ios::in);
		if (!fin) {
			std::cerr << "Cannot open " << fileName << std::endl;
			return false;
		}

		//std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)) {
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;
			if (cmd == "mtllib") {
				std::string mtlFileName;
				ss >> mtlFileName;
				std::string path;
				std::vector<std::string> paths = split(fileName, "/");
				for (int i = 0; i < paths.size() - 1; i++) {
					path += paths[i] + "/";
				}
				path += mtlFileName;
				std::cout << path << std::endl;
				this->loadMTL(path);
			}
			else if (cmd == "usemtl") {
				std::string materialName;
				ss >> materialName;
				for (int i = 0; i < Materials.size(); i++) {
					if (Materials[i].name == materialName) {
						materialId = i;
					}
				}
			}
			else if (cmd == "v") {
				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;

				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt") {
				glm::vec2 uv;
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;

				tempUVs.push_back(uv);
			}
			else if (cmd == "vn") {
				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;
				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			else if (cmd == "f") {
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex;

				while (ss >> faceData) {
					std::vector<std::string> data = split(faceData, "/");
					materialIndices.push_back(materialId);

					if (data[0].size() > 0) {
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}

					if (data.size() >= 1) {
						// Is face format v//vn?  If data[1] is empty string then
						// this vertex has no texture coordinate
						if (data[1].size() > 0)
						{
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}

					if (data.size() >= 2) {
						// Does this vertex have a normal?
						if (data[2].size() > 0)
						{
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}
			}
		}

		// Close the file
		fin.close();


		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			Vertex meshVertex;

			// Get the attributes using the indices

			if (tempVertices.size() > 0) {
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}

			if (tempNormals.size() > 0) {
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}

			if (tempUVs.size() > 0) {
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.UV = uv;
			}
			meshVertex.color = Materials[materialIndices[i]].color;
			meshVertex.specularColor = glm::vec3(1.0, 1.0, 1.0);
			meshVertex.ambientColor = Materials[materialIndices[i]].ambientColor;
			//std::cout << "Material to vertex - red: " << meshVertex.vertexmaterial.color.x << " green: " << meshVertex.vertexmaterial.color.y << " blue: " << meshVertex.vertexmaterial.color.r << std::endl;

			Vertices.push_back(meshVertex);
		}

		// Create and initialize the buffers
		initBuffers();

		return (isLoaded = true);
	}

	// We shouldn't get here so return failure
	return false;
}

void Mesh::setUVs(std::vector<glm::vec2> uvs) {
	for (int i = 0; i < uvs.size(); i++)
	{
		Vertices[i].UV = uvs[i];
	}

	initBuffers();
}

void Mesh::initBuffers(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	std::cout << "Initing buffer for: " << filename << " as: " << VAO << "&" << VBO << std::endl;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));

	// Vertex Positions
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);

}

void Mesh::enableAttribs() {
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

//-----------------------------------------------------------------------------
// Render the mesh
//-----------------------------------------------------------------------------

static GLuint lastVAO;
void Mesh::draw(){
	if (!isLoaded) return;
	if (VAO != lastVAO) {
		glBindVertexArray(0);
		lastVAO = VAO;
		glBindVertexArray(VAO);
		enableAttribs();
	}
	glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

}

void Mesh::cleanUp() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool Mesh::getLoaded() {
	return isLoaded;
}




