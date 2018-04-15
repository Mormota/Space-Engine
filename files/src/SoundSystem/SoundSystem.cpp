#include <stdio.h>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "openal\al.h"
#include "openal\alc.h"

#include "SoundSystem.h"


SoundSystem::SoundSystem(const char* file) {
	data = loadWAV(file, channel, sampleRate, bps, size);

	


	alGenBuffers(1, &bufferid);
	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}
	alBufferData(bufferid, format, data, size, sampleRate);

	alGenSources(1, &sourceid);
	alSourcei(sourceid, AL_BUFFER, bufferid);

	alSourcei(sourceid, AL_LOOPING, 1);

	
}

void SoundSystem::play() {
	alSourcePlay(sourceid);
}

char* SoundSystem::loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size){
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0){
		std::cout << "not valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}

bool SoundSystem::isBigEndian(){
	int a = 1;
	return !((char*)&a)[0];
}

int SoundSystem::convertToInt(char* buffer, int len){
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}
