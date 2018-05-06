#include "Text.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator> 
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "glm\gtx\transform.hpp"
#include "glm\glm.hpp"

std::vector<std::string> stringSplit(std::string s, std::string t)
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

std::vector<std::string> tokenSplit(std::string str, std::string chr) {
	std::istringstream buffer(str);
	std::vector<std::string> ret;

	std::copy(std::istream_iterator<std::string>(buffer),
		std::istream_iterator<std::string>(),
		std::back_inserter(ret));
	return ret;
	
}

std::string changeExtension(std::string path, std::string extension) {
	std::vector<std::string> paths = stringSplit(path, ".");
	return paths[0] +"." + extension;
}

FontFamily::FontFamily() {};
FontFamily::FontFamily(std::string filePath) {
	mesh.loadOBJ("res/models/gui.obj");
	texture.loadTexture(filePath);
	loadFontFamily(filePath);
}

void FontFamily::loadFontFamily(std::string filePath) {
	std::string atlas = changeExtension(filePath, "fnt");
	std::string fileName = atlas;

	std::vector<int> id, posx, posy, width, height, xOffset, yOffset, xAdvance;

	std::ifstream fin(fileName, std::ios::in);
	if (!fin) {
		std::cerr << "Cannot open " << fileName << std::endl;
		return;
	}


	std::string lineBuffer;
	std::cout << "Font atlas oppened: " << fileName << std::endl;
	while (std::getline(fin, lineBuffer)) {
		std::vector<std::string> tokens = tokenSplit(lineBuffer, "     ");
		Font font;
		for (int i = 0; i < tokens.size(); i++) {
			
			std::string token = tokens[i];
			std::vector<string> command = stringSplit(token, "=");
			std::string cmd, value = "NAN";
			if (command.size() > 1) {
				cmd = command[0];
				value = command[1];
			}
			if (cmd == "id") {
				font.id = std::stoi(value);
			}
			else if (cmd == "x") {
				font.x = std::stoi(value);
			}
			else if (cmd == "y") {
				font.y = std::stoi(value);
			}
			else if (cmd == "width") {
				font.width = std::stoi(value);
			}
			else if (cmd == "height") {
				font.height = std::stoi(value);
			}
			else if (cmd == "xoffset") {
				font.xOffset = std::stoi(value);
			}
			else if (cmd == "yoffset") {
				font.yOffset = std::stoi(value);
			}
			else if (cmd == "xadvance") {
				font.xAdvance = std::stoi(value);
				std::cout << token << std::endl;
			}

			
		}
		characters.push_back(font);
	}

	// Close the file
	fin.close();


	std::cout << characters.size() << std::endl;
	std::cout << characters[0].id << " - " << characters[characters.size() - 1].id << std::endl;
}

Font FontFamily::getFont(char font) {
	return getId((int)font);
}

Font FontFamily::getId(int id) {
	for (Font font : characters) {
		if (font.id == id)
			return font;
	}
}



Texture2D FontFamily::getTexture() {
	return texture;
}

Mesh FontFamily::getMesh() {
	return mesh;
}

Character::Character(FontFamily family, char chr, glm::vec2 displayDimensions) {
	font = family.getFont(chr);
	mesh = family.getMesh();
	this->family = family;

	glm::vec2 fontPixelSize = glm::vec2(font.width, font.height);
	fontScale = fontPixelSize / displayDimensions;
	fontOffset = glm::vec2(font.xOffset, font.yOffset) / displayDimensions;

	fontPosition = glm::vec2(fontScale.y, fontScale.x);




	//Texture coordinating
	int textureWidth = family.getTexture().getDimentsions().x;
	int textureHeight = family.getTexture().getDimentsions().y;

	glm::vec2 baselineposition = family.getTexture().getDimentsions() - glm::vec2(0, font.y);
	glm::vec2 topPosition = baselineposition - glm::vec2(font.width, font.height);

	//UV Coordinating
	std::vector<glm::vec2> uvs;

	glm::vec2 fontBase = glm::vec2(font.x, textureHeight - (font.y + font.height)) / family.getTexture().getDimentsions();
	glm::vec2 fontTop = glm::vec2(font.x + font.width, textureHeight - font.y) / family.getTexture().getDimentsions();

	glm::vec2 bottomLeft = glm::vec2(fontBase);
	glm::vec2 topleft = glm::vec2(fontBase.x, fontTop.y);
	glm::vec2 bottomRight = glm::vec2(fontTop.x, fontBase.y);
	glm::vec2 topRight = glm::vec2(fontTop);

	uvs.push_back(bottomRight);
	uvs.push_back(topleft);
	uvs.push_back(bottomLeft);
	uvs.push_back(bottomRight);
	uvs.push_back(topRight);
	uvs.push_back(topleft);

	mesh.setUVs(uvs);
}
static float cursor = 0;
void Character::render(ShaderProgram shader, int windowWidth, int windowHeight, float xOffset) {

	if (xOffset == 0)
		cursor = 0;
	cursor += abs(xOffset) * fontSize * 1.5 + font.xOffset;
	glm::vec2 displayDimensions = glm::vec2(windowWidth, windowHeight);
	glm::vec2 fontPixelSize = glm::vec2(font.width, font.height);
	fontScale = fontPixelSize / displayDimensions;
	fontScale *= fontSize;

	fontOffset = (glm::vec2(0, -font.yOffset) * fontSize * 2.0f) + glm::vec2(cursor , 0);

	fontPosition = glm::vec2(fontScale.x, -fontScale.y);

	glm::vec2 relativePos = glm::vec2(-1, 1);

	glm::vec2 position = fontPosition + (fontOffset + relativePos) / displayDimensions;

	glm::mat4 modelMatrix = glm::translate(glm::mat4(), glm::vec3(position, 0.0f)) * 
		glm::scale(glm::mat4(), glm::vec3(fontScale, 1.0f))  *
		glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setUniform("model", modelMatrix);
	shader.setUniform("textRenderer", true);
	shader.setUniform("textColor", textColor);
	shader.setUniform("textWidth", textWidth);
	shader.setUniform("textEdge", textEdge);
	shader.setUniform("textoutlineColor", textOutlineColor);
	shader.setUniform("textOutlineWidth", outlineWidth);
	shader.setUniform("textOutlineEdge", outlineEdge);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	family.getTexture().bind();
	mesh.draw();
	family.getTexture().unBind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	shader.setUniform("textRenderer", false);
}

Font Character::getFont() {
	return font;
}


