#include "PointLight.h"


PointLight::PointLight()
{
	DirectionalLight::DirectionalLight();
	_eyeDirection = vmath::vec3(0, 0, 0);
	_position = vmath::vec3(0, 1, 0);
	_constantAttenuation = 2.0;
	_linearAttenuation = 2.0;
	_quadraticAttenuation = 0.5;
}


PointLight::~PointLight()
{
}


void PointLight::setEyeDirection(vmath::vec3 direction)
{
	_eyeDirection = direction;
}
void PointLight::setPosition(vmath::vec3 position)
{
	_position = position;
}
void PointLight::setConstantAttenuation(float value) 
{
	_constantAttenuation = value;
}
void PointLight::setLinearAttenuation(float value) 
{
	_linearAttenuation = value;
}
void PointLight::setQuadraticAttenuation(float value) 
{
	_quadraticAttenuation = value;
}


void PointLight::render(Shader shader)
{
	DirectionalLight::render(shader);

	glUniform3f(shader.getUniformLocation("LightPosition"), _position[0], _position[1], _position[2]);
	glUniform3f(shader.getUniformLocation("EyeDirection"), _eyeDirection[0], _eyeDirection[1], _eyeDirection[2]);
	glUniform1f(shader.getUniformLocation("ConstantAttenuation"), _constantAttenuation);
	glUniform1f(shader.getUniformLocation("LinearAttenuation"), _linearAttenuation);
	glUniform1f(shader.getUniformLocation("QuadraticAttenuation"), _quadraticAttenuation);

	if (_on)
		glUniform1f(shader.getUniformLocation("pointLightingOn"), 1.0);
	else
		glUniform1f(shader.getUniformLocation("pointLightingOn"), 0.0);



}