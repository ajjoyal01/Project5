/*
	World.h
	Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
	Authors: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams, all took part in collaborating on functionality
	Date: 10/24/2014
	Course: CSC5210
	Description: World
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>
#include <queue>
#include <Windows.h>
#include <MMSystem.h>
#include <thread>

#include "LoadShaders.h"
#include "vgl.h"
#include "vmath.h"
#include "Object.h"
#include "Texture.h"
#include "Shader.h"
#include "Axes.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "ShadowMap.h"

#define NUM_TEXTURES 2 // cube and ground
#define CAM_MOVE .1
#define NUM_OBJECTS 4
#define NUM_LIGHTS 3
#define DIRECTIONAL 0
#define FLASHLIGHT 1
#define AMBIENT 2

using glm::mat4;

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
	void drawScene(Shader);
	void initValues();						// initializes values
	void setupTextures();
	void renderShadowMaps();

	void idleFunc();

	void createBlock();
	void animateBlockCreate();

private:

	Shader shader;
	Shader shadowMapShader;
	
	vec3 ambientLight;
	vector<Light*> lights;
	bool lighting_on;
	
	vector<Model*> cubes;

	ShadowMap shadowMap;
	bool shadow_maps_on;

	std::string textureFilenames[NUM_TEXTURES];
	Texture* textures[NUM_TEXTURES];

	Camera cam;

	bool drawAxes;
	Axes* axes;

	//--------------------------------
	// Lighting variables
	//--------------------------------
	float lightStrength;
	float lightShinniness;
	//--------------------------------

	Model terrain;
	Model shape;

	
	queue<int> animateQueue;

	int animate_amount;
	float animate_factor;
	float cube_travel_dist;
};

