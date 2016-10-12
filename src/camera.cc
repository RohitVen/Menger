#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix()
{
	return lookAt(eye_, look_, up_);
}

glm::mat4 Camera::lookAt(glm::vec3 eye, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 Z = eye - look;
	Z = glm::normalize(Z); 
	if((Z != glm::vec3(0.0) || up != glm::vec3(0.0)))
	{
		glm::vec3 Y = up;
		Y = glm::normalize(Y);
		glm::vec3 X = glm::cross(Y, Z);
		X = glm::normalize(X);
		Y = glm::cross(Z, X);

		glm::mat4 result(1);
		result[0][0] = X.x;
		result[1][0] = X.y;
		result[2][0] = X.z;
		result[3][0] = -1*glm::dot(X,eye);
		result[0][1] = Y.x;
		result[1][1] = Y.y;
		result[2][1] = Y.z;
		result[3][1] = -1*glm::dot(Y,eye);
		result[0][2] = Z.x;
		result[1][2] = Z.y;
		result[2][2] = Z.z;
		result[3][2] = -1*glm::dot(Z,eye);
		result[0][3] = 0;
		result[1][3] = 0;
		result[2][3] = 0;
		result[3][3] = 1;
		return result;
	}
	return glm::mat4(0);
}
