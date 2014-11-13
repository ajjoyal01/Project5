#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

#include "LoadShaders.h"
#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define EYE_MOVE_DEFAULT .1	
#define LOOK_MOVE_DEFAULT 2	// in degrees

//using glm::mat4;
using glm::vec3;

class Camera
{
public:
	Camera();
	~Camera();

	void camOut();
	void camIn();
	void camUp();
	void camDown();
	void camLeft();
	void camRight();

	void camLookLeft();
	void camLookRight();
	void camLookUp();
	void camLookDown();

	void setEyeMove(float);
	void setLookAtMove(float);

	void init(float,float,float);
	void reverse();
	void render(Shader);


private:

	float eye_x;
	float eye_y;
	float eye_z;

	float look_x;
	float look_y;
	float look_z;

	float eye_move;
	float look_move;

	glm::mat4 view;
	glm::mat4 lookAt;
	glm::mat4 position;
	glm::mat4 frustum;
	glm::mat4 rotate;
};

