#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix();
	glm::mat4 lookAt(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
	void drag(double x, double y, int winx, int winy);
	void zoom(double x, double y);
	void swap_fps();
	void wasd(int w, int a, int s, int d);
	void udlr(int u, int d, int l, int r);
	// FIXME: add functions to manipulate camera objects.
private:
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
	glm::vec3 center = eye_ + (camera_distance_ * look_);
	glm::vec3 tangent = glm::cross(look_, up_);
	double old_y = -1;
	double old_x = -1;
	int fps_mode = 0;
	glm::vec3 old_eye = eye_;
	glm::vec3 old_center = center;
	// Note: you may need additional member variables
};

#endif
