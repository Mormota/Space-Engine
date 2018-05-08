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
	int lineHeight, fontSize;
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
			if (cmd == "lineHeight") {
				lineHeight = std::stoi(value);
			}
			else if (cmd == "size") {
				fontSize = std::stoi(value);
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

			font.lineHeight = lineHeight;
			font.size = fontSize;

			
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

Character::Character(FontFamily family, char chr) {
	font = family.getFont(chr);
	mesh = family.getMesh();
	this->family = family;




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

void Character::setFontSize(float fontSize) {
	float relativeFontSize = fontSize / font.size;
	this->fontSize = relativeFontSize;
}
void Character::render(ShaderProgram shader, int windowWidth, int windowHeight, glm::vec2 relativeFontOffset) {
	
	glm::vec2 displayDimensions = glm::vec2(windowWidth, windowHeight);
	glm::vec2 fontPixelSize = glm::vec2(font.width, font.height);
	fontScale = fontPixelSize / displayDimensions;
	fontScale *= fontSize;

	fontOffset = (glm::vec2(font.xOffset, -font.yOffset) * fontSize * 2.0f);

	fontPosition = glm::vec2(fontScale.x, -fontScale.y);

	glm::vec2 relativePos = glm::vec2(0, 0);

	glm::vec2 position = fontPosition + relativePos + (fontOffset) / displayDimensions;

	position += relativeFontOffset / displayDimensions;

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

void Character::setColor(glm::vec3 color) {
	this->textColor = color;
}

std::vector<char> Word::getCharacters(std::string word) {
	std::vector<char> res;
	std::vector<char> chars(word.begin(), word.end());
	for (char Char : chars) {
		res.push_back(Char);

	}
	return res;
}

Word::Word(std::string word, FontFamily family, ShaderProgram shader, float textSize) {
	this->word = word;
	this->textSize = textSize;
	this->family = family;
	this->shader = shader;

	std::vector<char> chars = this->getCharacters(word);
	int wordLength = chars.size();
	float lenghtInPixels = 0;
	for (int i = 0; i < wordLength; i++) {
		Character character = Character(family, chars[i]);
		character.setFontSize(textSize);
		lenghtInPixels += character.getFont().xAdvance;
		characters.push_back(character);
	}

	wordLenghtInPixels = lenghtInPixels;
}

void Word::setColor(glm::vec3 color) {
	this->color = color;
}

float Word::getWidth() {
	return wordLenghtInPixels;
}

void Word::render(glm::vec2 wordPosition, glm::vec2 displayDimensions) {
	glm::vec2 position;
	int charNumber = characters.size();

	for (int i = 0; i < charNumber; i++) {
		if (i == 0) {
			position = wordPosition;
		}
		characters[i].setColor(color);
		characters[i].render(shader, displayDimensions.x, displayDimensions.y, position);
		position.x += characters[i].getFont().xAdvance * (textSize / characters[i].getFont().size) * 1.2f;
	}
}



Line::Line(FontFamily family, ShaderProgram shader, float textSize, float maxLenght) {
	spaceLenght = family.getFont(' ').xAdvance;
	this->family = family;
	this->shader = shader;
	this->textSize = textSize;
	this->maxLenght = maxLenght;
}

bool Line::addWord(Word word) {
	if (currentLineWidth + word.getWidth() < maxLenght) {
		words.push_back(word);
		currentLineWidth += word.getWidth();
		return true;
	}
	return false;
}

void Line::render(glm::vec2 linePosition, glm::vec2 displayDimensions) {
	int wordNumber = words.size();
	float wordOffset;
	for (int i = 0; i < wordNumber; i++) {
		if (i == 0)
			wordOffset = 0;
		words[i].setColor(color);
		words[i].render(linePosition + glm::vec2(wordOffset, 1), displayDimensions);
		wordOffset += words[i].getWidth() + spaceLenght;
	}
}

void Line::setColor(glm::vec3 color) {
	this->color = color;
}




std::vector<std::string> Text::getWords(std::string s) {
	char t = ' ';
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

Text::Text(std::string text, FontFamily family, ShaderProgram shader, float textSize, glm::vec2 textPosition, float textLenght) {
	this->family = family;
	this->shader = shader;
	this->textSize = textSize;
	this->textPosition = textPosition;
	this->lineHeight = family.getFont(' ').lineHeight;
	this->fontHeight = family.getFont(' ').size;
	std::cout << textSize / fontHeight << std::endl;
	
	std::vector<std::string> wordString = getWords(text);
	int wordNum = wordString.size();
	
	Line CurrentLine = Line(family, shader, textSize, textLenght);

	for (int i = 0; i < wordNum; i++) {
		Word word = Word(wordString[i], family, shader, textSize);
		if (CurrentLine.addWord(word)) {}
		else {
			lines.push_back(CurrentLine);
			CurrentLine = Line(family, shader, textSize, textLenght);
			CurrentLine.addWord(word);
		}
		if (i == wordNum - 1)
			lines.push_back(CurrentLine);
	}
}

void Text::render(glm::vec2 displayDimensions) {
	
	glm::vec2 linePosition = textPosition;

	int lineNumber = lines.size();
	for (int i = 0; i < lineNumber; i++) {
		lines[i].render(linePosition, displayDimensions);
		linePosition.y -= lineHeight;
	}
}

void Text::setColor(glm::vec3 color) {
	this->color = color;
	int lineNum = lines.size();
	for (int i = 0; i < lineNum; i++) {
		lines[i].setColor(color);
	}
}
