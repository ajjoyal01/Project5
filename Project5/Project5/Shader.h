/*
	Shader.h
	Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
	Authors: Greg Venezia
	Comments: most code recycled from project 4, updates by Andrew Joyal
	Date: 10/24/2014
	Course: CSC5210
	Description: Shader
*/

#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include <iostream>
#include "LoadShaders.h"

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	void init();
	void init(std::string vertPath, std::string fragPath);
	GLint getUniformLocation(std::string);
	void use();
	void unuse();


private:
	GLuint _id;


};

