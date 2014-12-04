#include "Camera.h"

Camera::Camera()
{	
}

Camera::~Camera()
{
}

void Camera::camOut()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, -eye_move));
	view = translate * view;
	eye_z += eye_move;
}

void Camera::camIn()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, eye_move));
	view = translate * view;
	eye_z -= eye_move;
}

void Camera::camUp()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(0, -eye_move, 0));
	view = translate * view;
	eye_y += eye_move;
}

void Camera::camDown()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(0, eye_move, 0));
	view = translate * view;
	eye_y -= eye_move;
}

void Camera::camLeft()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(eye_move, 0, 0));
	view = translate * view;
	eye_x -= eye_move;
}

void Camera::camRight()
{
	glm::mat4 translate = glm::translate(glm::mat4(), vec3(-eye_move, 0, 0));
	view = translate * view;
	eye_x += eye_move;
}

// rotate the view
void Camera::camLookLeft()
{
	glm::mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3((float)0, eye_y, (float)0));
	view = rotate * view;
}
void Camera::camLookRight()
{
	glm::mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3((float)0, eye_y, (float)0));
	view = rotate * view;
}

void Camera::camLookUp()
{
	glm::mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3(eye_x, (float)0, (float)0));
	view = rotate * view;
}

void Camera::camLookDown()
{
	glm::mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3(eye_x, (float)0, (float)0));
	view = rotate * view;
}

void Camera::setEyeMove(float move)
{
	eye_move = move;
}

void Camera::setLookAtMove(float move)
{
	look_move = move;
}

void Camera::init(float x, float y, float z)
{
	eye_move = EYE_MOVE_DEFAULT;
	look_move = LOOK_MOVE_DEFAULT;

	eye_x = x;
	eye_y = y;
	eye_z = z;

	view = glm::translate(glm::mat4(), vec3(-eye_x, -eye_y, -eye_z));

	frustum = glm::frustum((float)-0.2, (float)0.2, (float)-0.2, (float)0.2, (float)0.3, (float)10000);
}

void Camera::reverse()
{
	
}

// Sets up uniform for the View Projection Matrix
void Camera::render(Shader shader)
{
	glUniformMatrix4fv(shader.getUniformLocation("VPMatrix"), 1, GL_FALSE, glm::value_ptr(frustum * view));
	glUniformMatrix4fv(shader.getUniformLocation("VMatrix"), 1, GL_FALSE, glm::value_ptr(view));
}