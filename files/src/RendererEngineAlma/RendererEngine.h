#include "../ShaderProgram/ShaderProgram.h"
#include <iostream>
#include "glm\glm.hpp"


#ifndef RENDERERENGINE_H

#define RENDERERENGINE_H

class RendererEngine {
public:
	RendererEngine(ShaderProgram Loadedshader, glm::mat4 projectionMatrix);


private:
	ShaderProgram shader;
};

#endif // !RENDERERENGINE_H
