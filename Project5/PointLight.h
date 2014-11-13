#pragma once

#include "DirectionalLight.h"

#include "vmath.h"



class PointLight : public DirectionalLight
{
public:
	PointLight();
	~PointLight();


	void render(Shader);

	void setEyeDirection(vmath::vec3);
	void setPosition(vmath::vec3);
	void setConstantAttenuation(float);
	void setLinearAttenuation(float);
	void setQuadraticAttenuation(float);


private:
	vmath::vec3 _eyeDirection;
	vmath::vec3 _position;
	float _constantAttenuation;
	float _linearAttenuation;
	float _quadraticAttenuation;



};

