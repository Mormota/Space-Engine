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
	int x, y, width, height, xOffset, yOffset, xAdvance;
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
#define CHARACTER_G

class Character
{
public:
	Character(FontFamily family, char chr, glm::vec2 display);
	void render(ShaderProgram shader, int windowHeight, int windowWidth, float xOffset);
	Font getFont();
private:
	float fontSize = 1;
	glm::vec2 fontScale;
	glm::vec2 fontOffset;
	glm::vec2 fontPosition;

	glm::vec3 textColor = glm::vec3(1.0, 1.0, 1.0);
	float textWidth = 0.5;
	float textEdge = 0.01;

	glm::vec3 textOutlineColor = glm::vec3(0.5, 0.5, 0.5);
	float outlineWidth = 0.6;
	float outlineEdge = 0.01;

	Mesh mesh;
	Font font;
	FontFamily family;
};

#endif