#include "../ShaderProgram/ShaderProgram.h"
#include <iostream>
#include <vector>
#include "glm\glm.hpp"
#include "../Entity/Entity.h"
#include "../Camera/Camera.h"


#ifndef RENDERERENGINE_H

#define RENDERERENGINE_H

struct CameraVectors {
	glm::vec3 cameraPos;
	glm::vec3 cameraTargetPos;
	float cameraYaw;
};

class RendererEngine {
public:
	RendererEngine(ShaderProgram Loadedshader);
	void updateViewFrostum(Camera camera);

	void processEntity(Entity entity);

	void render(Entity entity);
	void textureRender(Entity entity);


private:
	std::vector<Entity> entities;
	CameraVectors vectors;
	ShaderProgram shader;
};

#endif // !RENDERERENGINE_H
