//Libraries
#define GLEW_STATIC
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <random>


#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "glm\gtc\matrix_transform.hpp"
#include "openal\al.h"
#include "openal\alc.h"

//Custom includes
#include "Display/Display.h"
#include "ShaderProgram/ShaderProgram.h"
#include "Model\Texture2D.h"
#include "Camera\Camera.h"
#include "Entity\Entity.h"
#include "gui\Gui.h"
#include "Model\Mesh.h"
#include "SoundSystem\SoundSystem.h"
#include "gui\Text.h"
#include "RendererEngine\RendererEngine.h"
#include "Entity\Planet.h"
#include "Content\Resource.h"
#include "Entity\Ship.h"

#define APIENTRY    WINAPI
using namespace glm;

int width = 1280, height = 720;
GLFWwindow* window = NULL;



bool wireFrame = false;
bool boundingBoxes = false;


enum gameStates {
	Initializing,
	inMainMenu,
	inIngameMenu,
	inPauseMenu,
	inHelpMenu,
	inGame,
	saving,
	quiting,
	credits,
	other
};
gameStates gameState = inGame;

struct PLANET {
	Entity data;
};

float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
float cameraRadius = 50.0f;
vec3 targetPosition(0.0f, 0.0f, 0.0f);

void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//AudioFile<double> audioFile;

void characterRender(std::vector<Character> characters, ShaderProgram shader, int windowWidth, int windowHeight) {
	static float xOffset = 0;
	int charNumber = characters.size();
	

	for (int i = 0; i < charNumber; i++) {
		if (i == 0) {
			xOffset = 0;
		}
		characters[i].render(shader, windowWidth, windowHeight, xOffset);
		xOffset = (float)characters[i].getFont().xAdvance;
	}
}

std::vector<Character> text(std::string text, FontFamily family) {
	std::vector<Character> res;
	std::vector<char> chars(text.begin(), text.end());
	for (char Char : chars) {
		Character character = Character(family, Char, glm::vec2(width, height));
		res.push_back(character);

	}

	static float xOffset = 0;
	int charNumber = res.size();
	for (int i = 0; i < charNumber; i++) {
		if (i == 0) {
			xOffset = 0;
		}

		std::cout << "ID: " << res[i].getFont().id << " - " << res[i].getFont().xAdvance << std::endl;

	}
	return res;
}


ALCdevice* device;
ALCcontext* context;

std::vector<Entity> entites;
std::vector<Planet> planetsTemp;
std::vector<Planet> planets;

Camera camera;

ShaderProgram shader;
ShaderProgram guiShader;


bool loadReady = false;
bool loadInProgress = false;

int main() {

	std::srand(std::time(nullptr));

	Display display = Display(width, height, "Space Engine");
	window = display.initOpenGl();
	if (window == NULL) return -1;
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetScrollCallback(window, scroll_callback);

	//openAL
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	if (device == NULL) std::cout << "cannot open sound card" << std::endl;
	if (context == NULL) std::cout << "cannot open context" << std::endl;
	alcMakeContextCurrent(context);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	//bgSound
	//SoundSystem bgMusic = SoundSystem("res/sounds/sound_1.wav");
	//bgMusic.setListenerPos(glm::vec3(1.0f, 1.0f, 1.0f));
	//bgMusic.setLooping(true);
	//bgMusic.play();


	//Shader init
	guiShader.loadShaders("shaders/guiVertex.glsl", "shaders/guiFragment.glsl");
	shader.loadShaders("shaders/vertex.hlsl", "shaders/fragment.hlsl");


	Resource apple;
	apple.setName("Apple");
	apple.setPrices(100, 125, 140);
	

	
	//loadEntities();

	

	Mesh planetMesh;
	planetMesh.loadOBJ("res/planetScheme.obj");
	Mesh moonMesh;
	moonMesh.loadOBJ("res/moonScheme.obj");

	Texture2D texture;
	texture.loadTexture("res/images/planet.png");


	Planet moon = Planet(moonMesh, texture, shader, 36);
	moon.setID(13);
	moon.setDistanceFromCenter(6);
	moon.setName("egyes típusú ûrbázis");
	moon.setOrbitalRotationSpeed(6);
	moon.setRotationSpeed(-6);
	moon.setCirculateAngle(5);
	moon.setOrbitalRotation(0);


	Planet earth = Planet(planetMesh, texture, shader, 36);
	earth.setName("Almacsutka 12");
	earth.setOrbitalCenter(glm::vec3(0, 0, 0));
	earth.setDistanceFromCenter(12);
	earth.setPosition(glm::vec3(0, 0, 0));
	earth.setCirculateAngle(6);
	earth.setOrbitalRotationSpeed(10);
	earth.setRotationSpeed(12);
	earth.setID(16);
	earth.setDistortion(0);
	//earth.addSubOrbit(moon);

	for (int i = 0; i < 20; i++) {
		planets.push_back(earth);
	}

	earth.setDistanceFromCenter(20);
	earth.setOrbitalRotationSpeed(14);
	moon.setDistanceFromCenter(4);
	moon.setOrbitalRotationSpeed(8);
	//moon.setOrbitAngle(12);
	moon.setOrbitalRotation(80);
	earth.addSubOrbit(moon);

	planets.push_back(earth);

	Mesh shipModel;
	shipModel.loadMatObj("res/models/tradeship.obj");


	Ship testShip = Ship(shipModel, shader);
	testShip.setPosition(glm::vec3(0, 0, 0 ));
	testShip.templateForces();

	std::cout << sizeof(std::vector<int>) + (sizeof(int) * planets.size()) << " - " << planets.size() << std::endl;

	FontFamily arial = FontFamily("res/fonts/depth/candara.png");
	std::vector<Character> charList = text("Lorem-ipsum dolor sit amet, consectetur adipiscing elit.", arial);

	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		display.getFrames();
		display.setDeltaTime();
		//display.setDeltaTime();
		glfwPollEvents();

		//Camera movement

		camera.setLookAt(targetPosition);
		camera.rotate(cameraYaw, cameraPitch);
		camera.setRadius(cameraRadius);

		//Camera usage
		display.initDisplay();
		display.useShader(shader, camera);


		double posX, posY;
		glfwGetCursorPos(window, &posX, &posY);

		//game state in game
		if (gameState == inGame) {

			int movingPlanet = 0;
			while (movingPlanet < planets.size()) {
				planets[movingPlanet].orbit(display.getDeltaTime());
				planets[movingPlanet].rotate(display.getDeltaTime());
				movingPlanet++;
			}

			testShip.updatePosition(display.getDeltaTime());


			
			

			


			
			if (display.mouseLeftPressed()) {

				//object picking
				int currentPickingPlanet = 0;
				while (currentPickingPlanet < planets.size()) {
					planets[currentPickingPlanet].planetPickingRender();
					currentPickingPlanet++;
				}

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
			if (!boundingBoxes) {
				display.initDisplay();
				shader.use();

				testShip.render();
				int currentPlanet = 0;
				while (currentPlanet < planets.size()) {
					planets[currentPlanet].textureRender();
					currentPlanet++;
				}
				
			}
			else {
				testShip.pickingRender();
				int currentPlanet = 0;
				while (currentPlanet < planets.size()) {
					planets[currentPlanet].planetPickingRender();
					currentPlanet++;
				}
			}
			characterRender(charList, shader, width, height);
			

		}	

		



		
		//Display updater
		shader.use();
		display.update();
	}

	for (Entity entity : entites) entity.getMesh().cleanUp();

	shader.cleanUp();
	guiShader.cleanUp();
	glfwTerminate();
	return 0;
}



void keyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mode) {

	if (gameState == inMainMenu) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) gameState = inGame;
		//loadEntities();
	} else if (gameState == inGame) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) gameState = inMainMenu;
	}

	
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS) {
		wireFrame = !wireFrame;
		if (wireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_F9 && action == GLFW_PRESS) boundingBoxes = !boundingBoxes;
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
		static vec2 lastPositionPress = vec2(0, 0);
		static bool travel = false;

		//UpdateAngles
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1) {
			cameraYaw -= ((float)posX - lastMousePos.x) * (float)mouseSensitivity;
			cameraPitch += ((float)posY - lastMousePos.y) * (float)mouseSensitivity;
			cameraPitch = glm::clamp(cameraPitch, -90.0f, +90.0f);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
			if (!travel) {
				lastPositionPress = vec2(posX, posY);
				travel = true;
			}
			else {

				float speedX = ((float)posX - lastPositionPress.x);
				float speedY = ((float)posY - lastPositionPress.y);

				float mouseDeltaY = ((float)posY - lastMousePos.y);
				float mouseDeltaX = ((float)posX - lastMousePos.x);
				float dz = speedX * cosf(radians(cameraYaw)) * mouseSensitivity / 10;
				float dx = speedY * sinf(radians(cameraYaw)) * mouseSensitivity / 10;
				targetPosition.z -= dz;
				targetPosition.x -= dx;

				//std::cout << speedX << "-" << cameraYaw << std::endl;
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 0) {
			if (travel) travel = false;
		}
		lastMousePos = vec2(posX, posY);
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	//std::cout << xoffset << " - " << yoffset << std::endl;
	cameraRadius -= yoffset * 5;
	cameraRadius = glm::clamp(cameraRadius, 10.0f, 500.0f);

}





