#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;

ShaderProgram::ShaderProgram():shaderHandle(0)
{
}


ShaderProgram::~ShaderProgram(){
	//glDeleteProgram(shaderHandle);
}

bool ShaderProgram::loadShaders(const char* vertexFileName, const char* fragmentFileName) {

	string vertexShaderString = fileToString(vertexFileName);
	string fragmentShaderString = fileToString(fragmentFileName);
	 
	const GLchar* vertexSourcePtr = vertexShaderString.c_str();
	const GLchar* fragmentSourcePtr = fragmentShaderString.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	shaderHandle = glCreateProgram();
	
	glShaderSource(vShader, 1, &vertexSourcePtr, NULL);
	glShaderSource(fShader, 1, &fragmentSourcePtr, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	checkCompileErrors(vShader, VERTEX);
	checkCompileErrors(fShader, FRAGMENT);
	
	glAttachShader(shaderHandle, vShader);
	glAttachShader(shaderHandle, fShader);

	glLinkProgram(shaderHandle);
	checkCompileErrors(shaderHandle, PROGRAM);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//UniformLocations.clear();

	return true;
}

void ShaderProgram::use() {
	if(shaderHandle > 0) glUseProgram(shaderHandle);
}

string ShaderProgram::fileToString(const string& filename) {
	std::stringstream ss;
	std::ifstream file;


	try{
		file.open(filename, std::ios::in);
		if (!file.fail()) ss << file.rdbuf();
		file.close();
	}
	catch (const std::exception ex){
		cerr << "Error reading shader file: " << endl;
	}

	return ss.str();
}
void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type) {
	int status = 0;
	GLint length = 0;
	if (type == PROGRAM) {
		glGetProgramiv(shaderHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			glGetProgramiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetProgramInfoLog(shaderHandle, length, &length, &errorLog[0]);
			std::cerr << "Program failed to link: " << errorLog << endl;
		}
	} else { //Shaders
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Shader failed to compile: " << errorLog << endl;
		}
	}
	GLint result;
	GLchar infoLog[512];


	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error: Vertex shader compile error: " << infoLog << std::endl;
	}
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) {
	GLint location = getUniformLocation(name);
	glUniform2f(location, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {
	GLint location = getUniformLocation(name);
	glUniform3f(location, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {
	GLint location = getUniformLocation(name);
	glUniform4f(location, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m) {
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

GLint ShaderProgram::getUniformLocation(const GLchar* name) {
	map<string, GLint>::iterator it = UniformLocations.find(name);

	if (it == UniformLocations.end()) UniformLocations[name] = glGetUniformLocation(shaderHandle, name);

	return UniformLocations[name];
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::setShaderProgram(GLuint program) {
	glUseProgram(program);
}

void ShaderProgram::cleanUp() {
	UniformLocations.clear();
	glDeleteProgram(shaderHandle);
}

GLuint ShaderProgram::getProgramID() {
	return shaderHandle;
}

GuiShader::GuiShader(const char* vertexFileName, const char* fragmentFileName) {
	ShaderProgram::loadShaders(vertexFileName, fragmentFileName);
}

void GuiShader::cleanUp() {
	ShaderProgram::cleanUp();
}

void GuiShader::use() {
	ShaderProgram::use();
}
