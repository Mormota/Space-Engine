#pragma once
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include "openal\al.h"
#include "openal\alc.h"

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW\glfw3.h"

#include "../Camera/Camera.h"
#include "../ShaderProgram/ShaderProgram.h"


#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

class SoundSystem {
public:
	SoundSystem();
	SoundSystem(const char* file);
	~SoundSystem();

	void loadSound(const char* file);

	void setListenerPos(glm::vec3 pos);
	void setListenerOri();

	void setVolume(float volume);
	void setPitch(float pitch);

	void setVelocity(glm::vec3 velocity);
	void setLooping(bool value);

	void setSoundPos(glm::vec3 pos);

	bool isPlaying();


	void pause();
	void resume();
	void stop();
	void play();

	void setDistanceGain();


	void cleanUp();
private:
	bool isBigEndian();
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
	int convertToInt(char* buffer, int len);

	int channel, sampleRate, bps, size;
	char* data;

	ALCdevice* device;
	ALCcontext* context;

	unsigned int bufferid, format;
	unsigned int sourceid;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	float volume = 1, pitch = 1;


};
#endif // !Initialize.h