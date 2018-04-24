#pragma once

#include "glm\glm.hpp"
#include "glm/gtx/transform.hpp"

#include "GL\glew.h"
#include <iostream>
#include "../Model/Mesh.h"
#include "../Model/Texture2D.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "Entity.h"
#include "../Content/Resource.h"

#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <vector>


enum planetType {
	CARBON_PLANET,
	CIRCUMBINARI_PLANET,
	CITY_PLANET,
	CORELESS_PLANET,
	DESERT_PLANET,
	DWARF_PLANET,
	EARTH_PLANET,
	EXO_PLANET,
	EYEBALL_PLANET,
	GAS_PLANET,
	HELIUM_PLANET,
	HOT_JUPITER,
	HOT_NEPTUNE,
	ICE_GIANT,
	ICE_PLANET,
	INNER_PLANET,
	LAVA_PLANET,
	MEGA_EARTH_PLANET,
	MESO_PLANET,
	MINOR_PLANET,
	OCEAN_PLANET,
	PLUTOID_PLANET,
	ROGUE_PLANET,
	TERRESTIAL_PLANET,
	TROJAN_PLANET,


	MOON,
	SUN,
	STATION
};


class Planet : public Entity{
public:
	Planet(Mesh model, Texture2D texture, ShaderProgram shader, int id, vector<Planet> subOrbits = vector<Planet>());

	//movement
	void rotate(double deltaTime);
	void orbit(double deltaTime);



	//setters
	void addSubOrbit(Planet orbit);
	void setPlanetAttributes(
		const string name, 
		float planetRotation, 
		float orbitalRotation,
		float planetRotationSpeed, 
		float orbitalRotationSpeed, 
		glm::vec3 orbitalCenter,
		float distanceFromCenter,
		float distortion
		);
	void setName(const string name);
	void setPlanetRotation(float rotation);
	void setOrbitalRotation(float orbitalRotation);
	void setOrbitalCenter(glm::vec3 centerPosition);
	void setDistanceFromCenter(float distanceFromCenter);
	void setRotationSpeed(float rotationSpeed);
	void setOrbitalRotationSpeed(float orbitalRotationSpeed);
	void setDistortion(float distortion);
	void setDistortionAngle(float angle);

	void setOrbitAngle(float angle);
	void setCirculateAngle(float angle);

	void setOffset(glm::vec3 offset);
	void setOffsetX(float offset);
	void setOffsetY(float offset);
	void setOffsetZ(float offset);

	void setType(planetType type);

	void planetRender();
	void planetPickingRender();
	void planetTextureRender();
	string getName();
	

private:
	string name = "Unnamed";
	float rotation = 0;
	float orbitRotation = 0;
	float angle = 0;
	float orbitAngle = 0;
	glm::vec3 orbitCenter = glm::vec3(0, 0, 0);
	float distanceFromCenter = 25;

	int rotationSpeed = 20;
	int orbitalSpeed = 36;

	float distortion = 0;
	float distortionAngle = 0;

	glm::vec3 offset = glm::vec3(0, 0, 0);

	vector<Planet> subOrbits;
	std::vector<Resource> resources;

	planetType type = EARTH_PLANET;


};

#endif // !PLANET_H

