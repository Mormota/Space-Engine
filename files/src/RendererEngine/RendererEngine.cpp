#include "./RendererEngine.h"



RendererEngine::RendererEngine(ShaderProgram Loadedshader){
	shader = Loadedshader;
	//shader.setUniform("projection", projectionMatrix);
}


void RendererEngine::render(Entity entity) {
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(glm::mat4(), entity.getPosition()) *
		glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f)) *
		glm::rotate(glm::mat4(), glm::radians(entity.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(entity.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(entity.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setUniform("model", modelMatrix);

	entity.getTexture().bind(0);
	entity.getMesh().draw();
	entity.getTexture().unBind();
}



/*
public class EntityRenderer {


	private StaticShader shader;

	public EntityRenderer(StaticShader shader, Matrix4f projectionMatrix) {
		this.shader = shader;
		shader.start();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
	}


	public void render(Map<TexturedModel, List<Entity>> entities) {
		for (TexturedModel model : entities.keySet()) {
			prepareTexturedModel(model);
			List<Entity> batch = entities.get(model);
			for (Entity entity : batch) {
				prepareinstances(entity);

				GL11.glDrawElements(GL11.GL_TRIANGLES, model.getRawModel().getVertexCount(), GL11.GL_UNSIGNED_INT, 0);
			}

			unbindTexturedModel();
		}
	}

	private void prepareTexturedModel(TexturedModel model) {
		RawModel rawModel = model.getRawModel();
		GL30.glBindVertexArray(rawModel.getVaoID());
		GL20.glEnableVertexAttribArray(0);
		GL20.glEnableVertexAttribArray(1);
		GL20.glEnableVertexAttribArray(2);
		ModelTexture texture = model.getTexture();

		if (texture.getTransparent()) {
			MasterRenderer.disableCulling();
		}
		shader.loadFakeLighting(texture.isUseFakeLighting());
		shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
		GL13.glActiveTexture(GL13.GL_TEXTURE0);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, model.getTexture().getID());
	}

	private void unbindTexturedModel() {
		MasterRenderer.enableCulling();
		GL20.glDisableVertexAttribArray(0);
		GL20.glDisableVertexAttribArray(1);
		GL20.glDisableVertexAttribArray(2);
		GL30.glBindVertexArray(0);
	}

	private void prepareinstances(Entity entity) {
		Matrix4f transformationMatrix = Maths.createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
		shader.loadTransformationMatrix(transformationMatrix);
	}

	/*
	public void render(Entity entity, StaticShader shader) {
	TexturedModel texturedModel = entity.getModel();
	RawModel model = texturedModel.getRawModel();
	GL30.glBindVertexArray(model.getVaoID());
	GL20.glEnableVertexAttribArray(0);
	GL20.glEnableVertexAttribArray(1);
	GL20.glEnableVertexAttribArray(2);

	Matrix4f transformationMatrix = Maths.createTransformationMatrix(entity.getPosition(), entity.getRotX(), entity.getRotY(), entity.getRotZ(), entity.getScale());
	shader.loadTransformationMatrix(transformationMatrix);

	ModelTexture texture = texturedModel.getTexture();
	shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());

	GL13.glActiveTexture(GL13.GL_TEXTURE0);
	GL11.glBindTexture(GL11.GL_TEXTURE_2D, texturedModel.getTexture().getID());
	GL11.glDrawElements(GL11.GL_TRIANGLES, model.getVertexCount(), GL11.GL_UNSIGNED_INT, 0);
	GL20.glDisableVertexAttribArray(0);
	GL20.glDisableVertexAttribArray(1);
	GL20.glDisableVertexAttribArray(2);
	GL30.glBindVertexArray(0);
	}


}
*/