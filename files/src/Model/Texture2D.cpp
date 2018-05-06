#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm\glm.hpp>
#include "stb\stb_image.h"

Texture2D::Texture2D():textureID(0) {}
Texture2D::~Texture2D() {

}

bool Texture2D::loadTexture(const string& fileName, bool generateMipMaps) {
	int width, height, components;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL) {
		std::cerr << "Error loading texture: '" << fileName << "'" << std::endl;
		return false;
	}
	//invertation
	this->width = width, this->height = height;
	int widthInBytes = width * 4;
	unsigned char * top = NULL;
	unsigned char * bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++){
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row - 1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}


	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	
	stbi_image_free(imageData);

	if (generateMipMaps) glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}
void Texture2D::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::unBind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2D::getID() {
	return textureID;
}

glm::vec2 Texture2D::getDimentsions() {
	return glm::vec2(width, height);
}