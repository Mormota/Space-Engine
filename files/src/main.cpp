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


ALCdevice* device;
ALCcontext* context;

std::vector<Entity> entites;
std::vector<Planet> planetsTemp;
std::vector<Planet> planets;

Camera camera;

ShaderProgram shader;
ShaderProgram guiShader;


RendererEngine renderer = RendererEngine(shader);

bool loadReady = false;
bool loadInProgress = false;

void loadEntities() {
	//alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	loadInProgress = true;

	Mesh planet;
	planet.loadOBJ("res/planetScheme.obj");

	Texture2D texture;
	texture.loadTexture("res/images/planet.png");

	SoundSystem sound = SoundSystem("res/sounds/bounce.wav");
	sound.setListenerPos(glm::vec3(1.0f, 1.0f, 1.0f));
	sound.setLooping(true);
	//sound.play();

	for (int i = 0; i < 1; i++) {
		Entity planeta = Entity(planet, texture, shader, 100);
		planeta.setPosition(glm::vec3(i * 6 - 6, 0.0f, 0.0f));
		planeta.setID(25);
		planeta.addSound("res/sounds/ships/basic_selfmade.wav");
		//planeta.playSound();

		Planet moon = Planet(planet, texture, shader, 36);
		//moon.setPosition(glm::vec3(0.0f, 0.0f, 25.0f));
		moon.setID(12);

		entites.push_back(planeta);
		planets.push_back(moon);

	}

	loadReady = true;
	loadInProgress = false;
}



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
	SoundSystem bgMusic = SoundSystem("res/sounds/sound_1.wav");
	bgMusic.setListenerPos(glm::vec3(1.0f, 1.0f, 1.0f));
	bgMusic.setLooping(true);
	//bgMusic.play();


	//Shader init
	guiShader.loadShaders("shaders/guiVertex.glsl", "shaders/guiFragment.glsl");
	shader.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");



	Gui randomGui = Gui("res/images/bg.jpg", guiShader);
	Gui exitGui = Gui("res/images/exit.png", guiShader);
	Gui play = Gui("res/images/gui/play.png", guiShader);

	Resource apple;
	apple.setName("Apple");
	apple.setPrices(100, 125, 140);
	

	
	//loadEntities();

	

	Mesh planet;
	planet.loadOBJ("res/planetScheme.obj");

	Texture2D texture;
	texture.loadTexture("res/images/planet.png");


	Planet station = Planet(planet, texture, shader, 36);
	//moon.setPosition(glm::vec3(0.0f, 0.0f, 25.0f));
	station.setID(13);
	station.setDistanceFromCenter(6);
	station.setName("egyes típusú ûrbázis");
	station.setOrbitalRotationSpeed(-20);
	station.setRotationSpeed(-60);
	station.setType(MOON);
	station.setCirculateAngle(5);
	station.setOrbitalRotation(0);

	Planet station2 = Planet(planet, texture, shader, 36);
	//moon.setPosition(glm::vec3(0.0f, 0.0f, 25.0f));
	station2.setID(14);
	station2.setDistanceFromCenter(3);
	station2.setName("egyes típusú ûrbázis");
	station2.setOrbitalRotationSpeed(-25);
	station2.setRotationSpeed(-25);
	station2.setScale(0.5);
	station2.setType(MOON);

	//station.addSubOrbit(station2);

	Planet moon = Planet(planet, texture, shader, 36);
	//moon.setPosition(glm::vec3(0.0f, 0.0f, 25.0f));

	moon.setName("Almacsutka 12");
	moon.setOrbitalCenter(glm::vec3(0, 0, 0));
	moon.setDistanceFromCenter(0);
	moon.setPosition(glm::vec3(0, 0, 0));
	moon.setCirculateAngle(6);
	moon.setOrbitalRotationSpeed(10);
	moon.setRotationSpeed(50);
	moon.setID(16);
	moon.setDistortion(0);
	moon.addSubOrbit(station);
	planets.push_back(moon);


	//Game Loop
	while (!glfwWindowShouldClose(window)) {

		display.getFrames();
		display.setDeltaTime();
		glfwPollEvents();
		randomGui.setDisplay(width, height, window);
		exitGui.setDisplay(width, height, window);
		play.setDisplay(width, height, window);

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

			for (int k = 0; k < planets.size(); k++) {
				Planet planet = planets[k];
				planet.rotate(display.getDeltaTime());
				planet.orbit(display.getDeltaTime());

				planets[k] = planet;
			};


			//object picking
			for (Planet planet: planets) {
				planet.planetPickingRender();
			}
			

			



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

				for (Planet planet : planets) {
					if ((int)planet.getID() == (int)pickedID)
						std::cout << planet.getName() << std::endl;
				}
			}
			


			//object rendering
			if (!boundingBoxes) {
				display.initDisplay();
				shader.use();

				
				

				for (Planet planet : planets) {
					planet.planetRender();
				}
				for (Entity planeta : entites) {
					planeta.getCamera(camera);
					planeta.render();
				}
				
			}
			

		}	

		



		//GUI
		randomGui.setDisplay(width, height, window);
		randomGui.scaleInPixels(600, 400);
		randomGui.positionInPixels(10, 10);

		exitGui.setDisplay(width, height, window);
		exitGui.scaleInPixels(60, 60);
		exitGui.positionInPixels(width - 70, 10);


		//GUI actions
		if (exitGui.onClick()) {
			if (bgMusic.isPlaying()) {
				bgMusic.pause();
			}
			else {
				bgMusic.resume();
			}
		}
		

		

		if (gameState == inGame) {
			/*if (randomGui.onClick()) {
				if (gameState == inGame) {
					gameState = inMainMenu;
				}
				else {
					if (!loadReady)
						loadEntities();
					gameState = inGame;
				}
			}*/
			randomGui.render();
			exitGui.render();
		}
		else if(gameState == inMainMenu) {
			play.render();
			/*if (play.onClick()) {
				std::cout << "starting" << std::endl;
				if (!loadReady)
					loadEntities();
				gameState = inGame;
			}*/
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
		loadEntities();
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
	cameraRadius = glm::clamp(cameraRadius, 10.0f, 250.0f);

}





