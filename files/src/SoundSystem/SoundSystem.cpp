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

SoundSystem::SoundSystem(){}

void SoundSystem::loadSound(const char* file) {
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

	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, position.x, position.y, position.z);

	alSourcef(sourceid, AL_GAIN, volume);
	alSourcef(sourceid, AL_PITCH, pitch);


	alListener3f(AL_VELOCITY, 0, 0, 0);


	alListenerfv(AL_ORIENTATION, listenerOri);



	


}

void SoundSystem::setDistanceGain() {

}

SoundSystem::SoundSystem(const char* file) {
	loadSound(file);
}

SoundSystem::~SoundSystem() {
	//cleanUp();
}

void SoundSystem::pause() {
	alSourcePause(sourceid);
}

void SoundSystem::resume() {
	alSourcePlay(sourceid);
}

void SoundSystem::stop() {
	alSourceStop(sourceid);
}


void SoundSystem::play() {
	stop();
	//alSourcePlay(sourceid);
	resume();
}

void SoundSystem::setVelocity(glm::vec3 velocity) {
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void SoundSystem::setLooping(bool value) {
	alSourcei(sourceid, AL_LOOPING, value ? 1 : 0);
}

bool SoundSystem::isPlaying() {
	ALenum state;
	alGetSourcei(sourceid, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void SoundSystem::setListenerOri() {

}

void SoundSystem::setListenerPos(glm::vec3 pos) {
	position = pos;
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void SoundSystem::setSoundPos(glm::vec3 pos) {
	alSource3f(sourceid, AL_POSITION, pos.x, pos.y, pos.z);
}

void SoundSystem::setVolume(float volume) {
	alSourcef(sourceid, AL_GAIN, volume);
}
void SoundSystem::setPitch(float pitch) {
	alSourcef(sourceid, AL_PITCH, pitch);
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

void SoundSystem::cleanUp() {
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);
}
