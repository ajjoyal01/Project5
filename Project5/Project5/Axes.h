/*
	Axes.h
	Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
	Author: Andrew Joyal, and Gregory Venezia
	Date: 10/24/2014
	Course: CSC5210
	Description: Axes
*/

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>

#include "vgl.h"
#include "Object.h"
#include "Shader.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUM_AXES 3			// number of axes (x,y,z)
#define NUM_VERTICES 2		// number of vertices per axis

//---------------------------------------------------------
// Vertex Array and Buffer
//---------------------------------------------------------
enum { AXES_VERTS, NUM_AXES_VAOS };
enum { AXES_BUFFER, NUM_AXES_BUFFERS };
//---------------------------------------------------------

using namespace std;
using glm::vec4;

class Axes
{

public:
	void draw(Shader);
	void init(vec4[NUM_AXES][NUM_VERTICES]);
	void setColor(Color color);
	void setIsTextured(bool);

private:

	vec4 _position[NUM_AXES][NUM_VERTICES];		//vertex data for the position of the axes
	Color color;	//vertex data for the color of the axes

	//---------------------------------------------------------
	// Vertex Arrays and Buffers
	//---------------------------------------------------------
	GLuint VAOs[NUM_AXES_VAOS];
	GLuint Buffers[NUM_AXES_BUFFERS];
	//---------------------------------------------------------

	//---------------------------------------------------------
	// Vertex Attributes
	//---------------------------------------------------------
	enum {
		vDirection, vTexel, vNormal, vTexture, vIsTextured, vIsTransformed,
		vModelMatrix0, vModelMatrix1, vModelMatrix2, vModelMatrix3, vColor
	};
	//---------------------------------------------------------

	GLuint isTransformed;
	int isTextured;
	glm::mat4 transform;
};

