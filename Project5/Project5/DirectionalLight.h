/*
	DirectonalLight.h
	Author: Gregory Venezia
	Date: 10/27/2014
	Course: CSC5210
	Description: A light pointing an a particular direction
*/


#pragma once

#include "Shader.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	void render(Shader shader);
	void init();

	void setAmbient(Color color);
	void setColor(Color color);
	void setDirection(vec3);
	void setStrength(float strength);
	void setShininess(float shininess);
	void toggle();
	bool isOn();



protected:

	Color _ambient;
	Color _color; // determines the color of the light
	vec3 _direction;
	vec3 _halfVector; // not sure what this does
	float _shininess;
	float _strength;
	bool _on;




};

