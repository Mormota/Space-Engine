#include "../ShaderProgram/ShaderProgram.h"
#include <iostream>
#include "glm\glm.hpp"
#include "../Entity/Entity.h"


#ifndef RENDERERENGINE_H

#define RENDERERENGINE_H

class RendererEngine {
public:
	RendererEngine(ShaderProgram Loadedshader);

	void render(Entity entity);


private:
	ShaderProgram shader;
};

#endif // !RENDERERENGINE_H
