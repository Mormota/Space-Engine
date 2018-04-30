#include "./RendererEngine.h"



RendererEngine::RendererEngine(ShaderProgram Loadedshader){
	shader = Loadedshader;
	//shader.setUniform("projection", projectionMatrix);
}

void RendererEngine::updateViewFrostum(Camera camera) {
	vectors.cameraPos = camera.getCamPos();
}

void RendererEngine::processEntity(Entity entity) {
	entities.push_back(entity);
}

void RendererEngine::render(Entity entity) {
	entity.render();
}

void RendererEngine::textureRender(Entity entity) {
	//entity.textureRender();
}