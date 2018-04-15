#pragma once

#include "GL\glew.h"
#include <string>
#include "stb\stb_image.h"
using namespace std;

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

class Texture2D {
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& fileName, bool generateMipMaps = true);

	GLuint getID();
	void bind(GLuint texUnit = 0);
	void unBind(GLuint texUnit = 0);

private:
	GLuint textureID;
};

#endif // !TEXTURE2D_H
