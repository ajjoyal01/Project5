#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include "vmath.h"
#include "vec.h"

using vmath::mat4;
using namespace std;

class Object
{

public:
	Object();
	~Object();

	virtual void scale(float) = 0;
	virtual void translate(float, float, float) = 0;
	virtual void rotate(float, vmath::vec3) = 0;
	virtual void updateCenter() = 0;

protected:

	vec4 center = 0;
};

