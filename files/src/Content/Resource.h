#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>


enum resourceType {
	SOLID,
	GAS,
	LIQUID,
	MECHANICS,
	SCRAP
};
class Resource
{
public:
	Resource();

	void setRarity(double rarity);
	void setName(std::string name);
	void setPrices(double minPrice, double avgPrice, double maxPrice);
	void setVolume(double volume);
	void setType(resourceType type);
	void setMass(double mass);
	void setID(int id = 0);
	void setTexture();
	void setDescription(std::string description);
	void addBuildIntoResource(Resource resource);
	void addBasedOnResource(Resource resource);

	std::string getName();
	std::string getDescription();
	double getRarity();
	double getVolume();
	double getMass();
	int getID();
	double getPrice(double changer);

	std::vector<Resource> getBasedOnResources();
	std::vector<Resource> getBuildsIntoResources();


	

	bool spawnRate();

	
private:

	void calculateRate();


	resourceType type;

	int seed = 0;
	double rarity = 0;
	double rate;
	double chance;
	int count = 0;

	double avgPrice, minPrice, maxPrice;
	double volume;
	double mass;

	int ID = 0;

	std::string description = "Lorem ipsum dolor sit amet";
	std::string name = "Unnamed resource";

	std::vector<Resource> buildsInto;
	std::vector<Resource> basedOn;

};


#endif // !RESOURCE_H

