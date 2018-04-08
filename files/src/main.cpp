//Libraries
#define GLEW_STATIC
#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "glm\gtc\matrix_transform.hpp"

//Custom includes
#include "Display/Display.h"
#include "ShaderProgram/ShaderProgram.h"
#include "Model\Texture2D.h"
#include "Camera\Camera.h"
#include "Entity\Entity.h"
#include "gui\Gui.h"
#include "Model\Mesh.h"
using namespace glm;

int width = 1280, height = 720;
GLFWwindow* window = NULL;
bool wireFrame = false;


enum gameStates {
	Initializing,
	inGame,
	inMenu
};
gameStates gameState = inMenu;



float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
float cameraRadius = 50.0f;

void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double posX, double posY);



int main() {
	Display display = Display(width, height, "Space Engine");
	window = display.initOpenGl();
	if (window == NULL) return -1;

	

	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	

	Camera camera;

	ShaderProgram shader;
	shader.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");

	ShaderProgram guiShader;
	guiShader.loadShaders("shaders/guiVertex.glsl", "shaders/guiFragment.glsl");

	

	Entity planet = Entity("res/planetScheme.obj", "res/images/planet.png", false, shader, 355);


	Gui randomGui = Gui("res/images/bg.jpg", guiShader);
	Gui exitGui = Gui("res/images/exit.png", guiShader);
	

	

	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		display.getFrames();
		glfwPollEvents();
		randomGui.setDisplay(width, height, window);
		exitGui.setDisplay(width, height, window);



		//Camera movement
		vec3 targetPosition(0.0f, 0.0f, 0.0f);
		camera.setLookAt(targetPosition);
		camera.rotate(cameraYaw, cameraPitch);
		camera.setRadius(cameraRadius);

		//Camera usage
		display.initDisplay();
		display.useShader(shader, camera);


		double posX, posY;
		glfwGetCursorPos(window, &posX, &posY);

		
		//GUI actions
		if (exitGui.onClick()) {
			std::cout << "Exit call sent" << std::endl;
			glfwSetWindowShouldClose(window, GL_TRUE);
		} else if (randomGui.onClick()) {
			if (gameState == inGame) {
				gameState = inMenu;
			}
			else {
				gameState = inGame;
			}
		};

		//game state in game
		if (gameState == inGame) {
			
			//object picking
			planet.setRotation(0, 0, 0);
			planet.pickingRender();

			if (display.mouseLeftPressed()) {
				glFlush();
				glFinish();

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				unsigned char data[4];
				glReadPixels(posX, height - posY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

				int pickedID =
					data[0] +
					data[1] * 256 +
					data[2] * 256 * 256;

				std::cout << pickedID << std::endl;
			}

			


			//object rendering
			display.initDisplay();
			planet.render();

		}	



		//GUI
		randomGui.setDisplay(width, height, window);
		randomGui.scaleInPixels(600, 400);
		randomGui.positionInPixels(10, 10);

		exitGui.setDisplay(width, height, window);
		exitGui.scaleInPixels(60, 60);
		exitGui.positionInPixels(width - 70, 10);

		randomGui.render();
		exitGui.render();


		//Display updater
		shader.use();
		display.update();
	}
	shader.cleanUp();
	guiShader.cleanUp();
	glfwTerminate();
	return 0;
}



void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode) {

	if (gameState == inMenu) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) gameState = inGame;
	} else if (gameState == inGame) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) gameState = inMenu;
	}

	
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS) {
		wireFrame = !wireFrame;
		if (wireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void resizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double posX, double posY) {
	if (gameState == inGame) {
		const float mouseSensitivity = 0.25f;

		static vec2 lastMousePos = vec2(0, 0);
		//UpdateAngles
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1) {
			cameraYaw -= ((float)posX - lastMousePos.x) * (float)mouseSensitivity;
			cameraPitch += ((float)posY - lastMousePos.y) * (float)mouseSensitivity;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
			cameraRadius -= ((float)posY - lastMousePos.y) * (float)mouseSensitivity;
			cameraRadius = glm::clamp(cameraRadius, 10.0f, 250.0f);
		}
		lastMousePos = vec2(posX, posY);
	}

}





