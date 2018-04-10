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
	SoundSystem(const char* file);
	void play();
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


};
#endif // !Initialize.h