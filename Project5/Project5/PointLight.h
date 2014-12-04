#pragma once

#include "DirectionalLight.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;

class PointLight : public DirectionalLight
{
public:
	PointLight();
	~PointLight();


	void render(Shader);

	void setEyeDirection(vec3);
	void setPosition(vec3);
	void setConstantAttenuation(float);
	void setLinearAttenuation(float);
	void setQuadraticAttenuation(float);


private:
	vec3 _eyeDirection;
	vec3 _position;
	float _constantAttenuation;
	float _linearAttenuation;
	float _quadraticAttenuation;



};

