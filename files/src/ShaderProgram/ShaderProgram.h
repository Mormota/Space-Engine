#pragma once

#include <string>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <map>

using namespace std;

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H


class ShaderProgram{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vertexFileName, const char* fragmentFileName);
	void use();
	void cleanUp();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const float value);
	void setUniform(const GLchar* name, const bool value);

	void setUniform(const GLchar* name, const glm::mat4& v);

	GLuint getProgramID();

	void stop();
	void setShaderProgram(GLuint program);

private:
	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);

	GLint getUniformLocation(const GLchar* name);

	GLuint shaderHandle;
	map<string, GLint> UniformLocations;
};

class GuiShader : public ShaderProgram {
public:
	GuiShader(const char* guiVertexFileName, const char* guiFragmentFileName);

	void use();
	void cleanUp();

};

#endif // SHADER_PROGRAM_H
