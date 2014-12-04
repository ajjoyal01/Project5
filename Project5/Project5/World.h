#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>

#include "LoadShaders.h"
#include "vgl.h"
#include "vmath.h"
#include "Object.h"
#include "Texture.h"
#include "Shader.h"
#include "Axes.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Camera.h"
//#include "Game.h"
//#include "Room.h"

#define NUM_TEXTURES 3 // cards and table
#define CAM_MOVE .1
#define NUM_OBJECTS 4

using vmath::mat4;

static class World
{

public:

	World();
	~World();
	void init();							// initializes World
	void display();							// display function
	void keyPress(unsigned char, int, int);
	void arrowInput(int, int, int);
	void draw();							// draw function for entire World
	void initValues();						// initializes values
	void setupTextures();
	
	void idleFunc();

private:
	
	Shader shader;
	PointLight light;

	std::string textureFilenames[NUM_TEXTURES];
	Texture * textures[NUM_TEXTURES];

	Camera cam;

	bool drawAxes;
	Axes* axes;

	//--------------------------------
	// Lighting variables
	//--------------------------------
	float lightStrength;
	float lightShinniness;
	vmath::vec3 lightDirection;

	Color directionalColor;
	Color ambientColor;
	//--------------------------------

	Model terrain;
};

