/*
	Texture.h
	Group: Andrew Joyal, Hunter Ripsom-Gardiner, Connor Williams
	Authors: Greg Venezia
	Date: 10/24/2014
	Course: CSC5210
	Description: Texture
*/

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <vector>

#include "vgl.h"

#include "lodepng.h"
using namespace std;
class Texture
{
	friend class TextureCache;
public:
	Texture();
	~Texture();

	void loadFromFile(std::string filename);
	void load();
	void activate(GLint uniformLocation);
	void deactivate();

private:
	GLuint _id;

	unsigned int _width;
	unsigned int _height;

	std::vector<unsigned char> _image;

	std::string path;
};