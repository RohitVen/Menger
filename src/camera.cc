#define GLM_FORCE_RADIANS

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
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
	return lookAt(eye_, center, up_);
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

void Camera::drag(double x, double y, int winx, int winy)
{
	if(old_x == -1 && old_y == -1)
	{
		old_x = x;
		old_y = y;
	}
	else
	{
		float delx = old_x - x;
		float dely = old_y - y; 

		glm::vec3 rot = -dely*tangent + delx*up_;
		glm::mat4 result = glm::rotate(rotation_speed, rot);

		glm::vec4 final_up(up_, 1.0f);
		final_up = result * final_up;
		up_ = glm::vec3(final_up.x, final_up.y, final_up.z);
		if(glm::length(up_) != 0)
		{
			up_ = glm::normalize(up_);
		}

		glm::vec4 final_look(look_, 1.0f);
		final_look = result * final_look;
		look_ = glm::vec3(final_look.x, final_look.y, final_look.z); 
		if(glm::length(look_) != 0)
		{
			look_ = glm::normalize(look_);
		}

		tangent = glm::cross(look_, up_);
		if(!fps_mode)
		{
			eye_ = center - (camera_distance_ * look_);
		}
		else
		{
			center = eye_ + (camera_distance_ * look_);
		}
		old_y = y;
		old_x = x;
	}
}

void Camera::zoom(double x, double y)
{
	if(old_y == -1)
	{
		old_y = y;
	}
	if(y < old_y)
	{
		camera_distance_ -= zoom_speed;
		eye_ += (zoom_speed * look_);
		center += (zoom_speed * look_);
	}
	if(y > old_y)
	{
		camera_distance_ += zoom_speed;
		eye_ -= (zoom_speed * look_);
		center -= (zoom_speed * look_);
	}
	old_y = y;
}

void Camera::wasd(int w, int a, int s, int d)
{
	if(!fps_mode && w)
	{
		camera_distance_ -= zoom_speed;
		eye_ += (zoom_speed * look_);
		center += (zoom_speed * look_);
	}
	if(!fps_mode && s)
	{
		camera_distance_ += zoom_speed;
		eye_ -= (zoom_speed * look_);
		center -= (zoom_speed * look_);
	}
	if(!fps_mode && a)
	{
		eye_ -= (pan_speed * tangent);
		center = eye_ + (camera_distance_ * look_);
	}
	if(!fps_mode && d)
	{
		eye_ += (pan_speed * tangent);
		center = eye_ + (camera_distance_ * look_);
	}
	if(fps_mode && w)
	{
		eye_ += (zoom_speed * look_);
		center += (zoom_speed * look_);
	}
	if(fps_mode && s)
	{
		eye_ -= (zoom_speed * look_);
		center -= (zoom_speed * look_);
	}
	if(fps_mode && a)
	{
		eye_ -= (pan_speed * tangent);
		center = eye_ + (camera_distance_ * look_);
	}
	if(fps_mode && d)
	{
		eye_ += (pan_speed * tangent);
		center = eye_ + (camera_distance_ * look_);
	}
}

void Camera::udlr(int u, int d, int l, int r)
{
	if(!fps_mode && u)
	{
		center += (pan_speed * up_);
	}
	if(!fps_mode && d)
	{
		center -= (pan_speed * up_);
	}
	if(!fps_mode && l)
	{
		double unewx = (up_.x * glm::cos(roll_speed)) - (up_.y * glm::sin(roll_speed));
		double unewy = (up_.x * glm::sin(roll_speed)) + (up_.y * glm::cos(roll_speed));

		up_.x = unewx;
		up_.y = unewy;
		up_ = glm::normalize(up_);
		glm::vec3 tangent = glm::cross(look_, up_);
	}
	if(!fps_mode && r)
	{
		double unewx = (up_.x * glm::cos(-1*roll_speed)) - (up_.y * glm::sin(-1*roll_speed));
		double unewy = (up_.x * glm::sin(-1*roll_speed)) + (up_.y * glm::cos(-1*roll_speed));

		up_.x = unewx;
		up_.y = unewy;
		up_ = glm::normalize(up_);
		glm::vec3 tangent = glm::cross(look_, up_);
	}
	if(fps_mode && u)
	{
		eye_ += (pan_speed * up_);
		center = eye_ + (camera_distance_ * look_);
	}
	if(fps_mode && d)
	{
		eye_ -= (pan_speed * up_);
		center = eye_ + (camera_distance_ * look_);
	}
	if(fps_mode && l)
	{
		double unewx = (up_.x * glm::cos(roll_speed)) - (up_.y * glm::sin(roll_speed));
		double unewy = (up_.x * glm::sin(roll_speed)) + (up_.y * glm::cos(roll_speed));

		up_.x = unewx;
		up_.y = unewy;
		up_ = glm::normalize(up_);
		glm::vec3 tangent = glm::cross(look_, up_);
	}
	if(fps_mode && r)
	{
		double unewx = (up_.x * glm::cos(-1*roll_speed)) - (up_.y * glm::sin(-1*roll_speed));
		double unewy = (up_.x * glm::sin(-1*roll_speed)) + (up_.y * glm::cos(-1*roll_speed));

		up_.x = unewx;
		up_.y = unewy;
		up_ = glm::normalize(up_);
		glm::vec3 tangent = glm::cross(look_, up_);
	}

}

void Camera::swap_fps()
{
	fps_mode = !fps_mode;
}
