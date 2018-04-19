#include "Resource.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>

double generateRandom() {
	double random = (double)std::rand() / (RAND_MAX);
	return random;
}


Resource::Resource() {
	this->calculateRate();
}

void Resource::calculateRate() {
	rate = 1/rarity;
	chance = rate;
	if (rarity == 0) {
		chance = 0;
	}
}

bool Resource::spawnRate() {
	double chanceNumber = generateRandom();
	if (chanceNumber <= chance) {
		count++;
		chance = rate;
		seed = 0;
		return true;
	}
	else {
		seed++;
		return false;
	}

	
}




//-------------------------------------
//Setters
//-------------------------------------
void Resource::setRarity(double rarity) {
	this->rarity = rarity;
}
void Resource::setName(std::string name) {
	this->name = name;
}
void Resource::setPrices(double minPrice, double avgPrice, double maxPrice) {
	this->minPrice = minPrice;
	this->maxPrice = maxPrice;
	this->avgPrice = avgPrice;
}
void Resource::setVolume(double volume) {
	this->volume = volume;
}
void Resource::setType(resourceType type) {
	this->type = type;
}
void Resource::setMass(double mass) {
	this->mass = mass;
}
void Resource::setID(int id) {
	this->ID = id;
}
void Resource::setTexture() {
	//TODO
}

void Resource::setDescription(std::string description) {
	this->description = description;
}

void Resource::addBuildIntoResource(Resource resource) {
	buildsInto.push_back(resource);
}
void Resource::addBasedOnResource(Resource resource) {
	basedOn.push_back(resource);
}

//-------------------------------------
//Getters
//-------------------------------------
std::string Resource::getName() {
	return name;
}
double Resource::getRarity() {
	return rarity;
}
double Resource::getVolume() {
	return volume;
}
double Resource::getMass() {
	return mass;
}
int Resource::getID() {
	return ID;
}
double Resource::getPrice(double changer) {
	if (changer >= 0) {
		double difference = (maxPrice - avgPrice) * changer;
		return avgPrice + difference;
	}
	else {
		double difference = (avgPrice - minPrice) * abs(changer);
		return avgPrice - difference;
	}
}
std::string Resource::getDescription() {
	return description;
}

std::vector<Resource> Resource::getBasedOnResources() {
	return basedOn;
}
std::vector<Resource> Resource::getBuildsIntoResources() {
	return buildsInto;
}




