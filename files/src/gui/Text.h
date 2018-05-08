#ifndef FONTFAMILY_H
#define FONTFAMILY_H
#include <iostream>
#include <vector>
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"

struct Font
{
	int id;
	int x, y, width, height, xOffset, yOffset, xAdvance, lineHeight, size;
};

class FontFamily
{
public:
	FontFamily();
	FontFamily(std::string filePath);
	Font getId(int id);
	Font getFont(char font);

	Texture2D getTexture();
	Mesh getMesh();

private:
	Mesh mesh;
	void loadFontFamily(std::string fontFamily);
	std::vector<Font> characters;
	Texture2D texture;
};




#endif

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
public:
	Character(FontFamily family, char chr);
	void render(ShaderProgram shader, int windowHeight, int windowWidth, glm::vec2 relativeOffset);
	void setColor(glm::vec3 color);
	void setFontSize(float fontSize);
	Font getFont();
private:
	float fontSize = 1;
	glm::vec2 fontScale;
	glm::vec2 fontOffset;
	glm::vec2 fontPosition;

	glm::vec3 textColor = glm::vec3(1.0, 1.0, 1.0);
	float textWidth = 0.5;
	float textEdge = 0.1;

	glm::vec3 textOutlineColor = glm::vec3(1.0, 1.0, 1.0);
	float outlineWidth = 0.3;
	float outlineEdge = 0.01;

	Mesh mesh;
	Font font;
	FontFamily family;
};

#endif

#ifndef WORD_H
#define WORD_H

class Word
{
public:
	Word(std::string word, FontFamily family, ShaderProgram shader, float textSize);
	void setColor(glm::vec3 color);
	float getWidth();
	
	void render(glm::vec2 wordPosition, glm::vec2 displayDimensions);
private:
	std::vector<char> getCharacters(std::string word);

	std::string word;
	FontFamily family;
	ShaderProgram shader;
	float wordLenghtInPixels;
	float textSize;
	std::vector<Character> characters;

	glm::vec3 color;
};

#endif

#ifndef LINE_H
#define LINE_H

class Line
{
public:
	Line(FontFamily family, ShaderProgram shader, float textSize, float maxLenght);
	void setColor(glm::vec3 color);
	float getLineWidth;
	bool addWord(Word);
	void render(glm::vec2 linePosition, glm::vec2 displayDimensions);
private:
	
	glm::vec3 color = glm::vec3(1,1,1);
	float spaceLenght;
	ShaderProgram shader;
	FontFamily family;
	float textSize;
	float currentLineWidth;
	float maxLenght;
	std::vector<Word> words;
};

#endif

#ifndef TEXT_H
#define TEXT_H



class Text
{
public:
	Text(std::string text, FontFamily family, ShaderProgram shader, float textSize, glm::vec2 textPosition, float textLenght);
	void setColor(glm::vec3 color);

	void render(glm::vec2 displayDimensions);
private:
	glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f);
	std::vector<std::string> getWords(std::string string);

	std::vector<Word> words;
	std::vector<Line> lines;

	float lineHeight;
	float fontHeight;
	std::string text;
	FontFamily family;
	ShaderProgram shader;
	float textSize, textLenght;
	glm::vec2 textPosition;
};

#endif