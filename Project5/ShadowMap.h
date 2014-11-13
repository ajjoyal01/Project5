#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\freeglut.h>

#include <vmath.h>


class ShadowMap
{


public:
	ShadowMap();
	~ShadowMap();



	void render(); // set up uniforms
	void init(); 

private:
	GLuint _textureID;
	GLuint _fboID;




};

