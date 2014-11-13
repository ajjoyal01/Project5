#include "DirectionalLight.h"


DirectionalLight::DirectionalLight() : _on(false)
{
	_ambient.red = 0.0;
	_ambient.green = 0.0;
	_ambient.blue = 0.0;
	_direction = vmath::vec3(1.0f, -1.0, 1.0);
	_halfVector = vmath::vec3(0.0f, 0.0f, 0.0f);
	_strength = 1.0;
	_shininess = 1.0;
	_color.red = 1.0;
	_color.blue = 1.0;
	_color.green = 1.0;
}


DirectionalLight::~DirectionalLight()
{
}


void DirectionalLight::setAmbient(Color color)
{
	_ambient = color;
}
void DirectionalLight::setColor(Color color)
{
	_color = color;
}

void DirectionalLight::setDirection(vmath::vec3 direction)
{
	_direction = direction;
}
void DirectionalLight::setStrength(float strength)
{
	_strength = strength;
}
void DirectionalLight::setShininess(float shininess)
{
	_shininess = shininess;
}

void DirectionalLight::toggle()
{
	_on = !_on;
}

bool DirectionalLight::isOn()
{
	return _on;
}

void DirectionalLight::render(Shader shader)
{
	glUniform3f(shader.getUniformLocation("Ambient"), _ambient.red, _ambient.green, _ambient.blue);
	glUniform3f(shader.getUniformLocation("LightColor"), _color.red, _color.green, _color.blue);
	glUniform3f(shader.getUniformLocation("LightDirection"), _direction[0], _direction[1], _direction[2]);
	glUniform3f(shader.getUniformLocation("HalfVector"), _halfVector[0], _halfVector[1], _halfVector[2]);
	glUniform1f(shader.getUniformLocation("Shininess"), _shininess);
	glUniform1f(shader.getUniformLocation("Strength"), _strength);


	if (_on)
		glUniform1f(shader.getUniformLocation("directionalLightingOn"), 1.0);
	else
		glUniform1f(shader.getUniformLocation("directionalLightingOn"), 0.0);
}
