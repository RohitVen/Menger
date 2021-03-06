#include "menger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
	int index = 0;
}

Menger::~Menger() 
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
			  std::vector<glm::vec4>& vtx_normals,
                          std::vector<glm::uvec3>& obj_faces)
{	
	face_index = 0;
	double minx=-0.5, miny=-0.5, minz=-0.5, maxx=0.5, maxy=0.5, maxz=0.5;
	drawSponge(obj_vertices, obj_faces, vtx_normals, nesting_level_,
	minx, miny, minz, 
	maxx, maxy, maxz);
}

void Menger::drawCube(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces,
	double minx, double miny, double minz, 
	double maxx, double maxy, double maxz)
{
	//Defining all the vertices
	glm::vec4 vertPos[8] = {
		glm::vec4(minx, miny, maxz, 1.0), //Front bottom left
		glm::vec4(minx, maxy, maxz, 1.0), //Front top left
		glm::vec4(maxx, maxy, maxz, 1.0), //Front top right
		glm::vec4(maxx, miny, maxz, 1.0), //Front bottom right
		glm::vec4(minx, miny, minz, 1.0), //Back bottom left 
		glm::vec4(minx, maxy, minz, 1.0), //Back top left
		glm::vec4(maxx, maxy, minz, 1.0), //Back top right
		glm::vec4(maxx, miny, minz, 1.0)  //Back bottom right
	};

	//Loading the vertices into the VBO
	obj_vertices.push_back(vertPos[7]);
	obj_vertices.push_back(vertPos[4]);
	obj_vertices.push_back(vertPos[5]);
	obj_vertices.push_back(vertPos[5]);
	obj_vertices.push_back(vertPos[6]);
	obj_vertices.push_back(vertPos[7]);

	obj_vertices.push_back(vertPos[3]);
	obj_vertices.push_back(vertPos[2]);
	obj_vertices.push_back(vertPos[6]);
	obj_vertices.push_back(vertPos[6]);
	obj_vertices.push_back(vertPos[7]);
	obj_vertices.push_back(vertPos[3]);

	obj_vertices.push_back(vertPos[7]);
	obj_vertices.push_back(vertPos[4]);
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[3]);
	obj_vertices.push_back(vertPos[7]);
	
	obj_vertices.push_back(vertPos[2]);
	obj_vertices.push_back(vertPos[1]);
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[3]);
	obj_vertices.push_back(vertPos[2]);

	obj_vertices.push_back(vertPos[1]);
	obj_vertices.push_back(vertPos[5]);
	obj_vertices.push_back(vertPos[4]);
	obj_vertices.push_back(vertPos[4]);
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[1]);

	obj_vertices.push_back(vertPos[6]);
	obj_vertices.push_back(vertPos[5]);
	obj_vertices.push_back(vertPos[1]);
	obj_vertices.push_back(vertPos[1]);
	obj_vertices.push_back(vertPos[2]);
	obj_vertices.push_back(vertPos[6]);

	//Loading faces into the VBO
	obj_faces.push_back(glm::uvec3(face_index, face_index+1, face_index+2)); //Back face
	obj_faces.push_back(glm::uvec3(face_index+3, face_index+4, face_index+5)); //Back face
	obj_faces.push_back(glm::uvec3(face_index+6, face_index+7, face_index+8)); //Right face
	obj_faces.push_back(glm::uvec3(face_index+9, face_index+10, face_index+11)); //Right face
	obj_faces.push_back(glm::uvec3(face_index+12, face_index+13, face_index+14)); //Bottom face
	obj_faces.push_back(glm::uvec3(face_index+15, face_index+16, face_index+17)); //Bottom face
	obj_faces.push_back(glm::uvec3(face_index+18, face_index+19, face_index+20)); //Front face
	obj_faces.push_back(glm::uvec3(face_index+21, face_index+22, face_index+23)); //Front face
	obj_faces.push_back(glm::uvec3(face_index+24, face_index+25, face_index+26)); //Left face
	obj_faces.push_back(glm::uvec3(face_index+27, face_index+28, face_index+29)); //Left face
	obj_faces.push_back(glm::uvec3(face_index+30, face_index+31, face_index+32)); //Top face
	obj_faces.push_back(glm::uvec3(face_index+33, face_index+34, face_index+35)); //Top face
	face_index += 36;
}

void Menger::drawSponge(std::vector<glm::vec4>& obj_vertices, 
	std::vector<glm::uvec3>& obj_faces, 
	std::vector<glm::vec4>& vtx_normals,
	int level,
	double minx, double miny, double minz, 
	double maxx, double maxy, double maxz)
{

	double midx = (maxx - minx)/3;
	double midy = (maxy - miny)/3;
	double midz = (maxz - minz)/3;

	double xvals[4] = {minx, minx+midx, minx+2*midx, maxx};
	double yvals[4] = {miny, miny+midy, miny+2*midy, maxy};
	double zvals[4] = {minz, minz+midz, minz+2*midz, maxz};

	//Base case
	if(level == 0)
	{
		drawCube(obj_vertices, obj_faces,
			minx, miny, minz,
			maxx, maxy, maxz);

		for(int i=0; i<6; i++)
		{
			vtx_normals.push_back(glm::vec4(0.0, 0.0, -1.0, 0.0)); //Back Normals
		}
		for(int i=0; i<6; i++)
		{
			vtx_normals.push_back(glm::vec4(1.0, 0.0, 0.0, 0.0)); //Right Normals
		}
		for(int i=0; i<6; i++)
		{
			vtx_normals.push_back(glm::vec4(0.0, -1.0, 0.0, 0.0)); //Bottom Normals
		}
		for(int i=0; i<6; i++)
		{
			vtx_normals.push_back(glm::vec4(0.0, 0.0, 1.0, 0.0)); //Front Normals
		}
		for(int i=0; i<6; i++)
		{
			vtx_normals.push_back(glm::vec4(-1.0, 0.0, 0.0, 0.0)); //Left Normals
		}
		for(int i=0; i<6; i++)
		{
		 	vtx_normals.push_back(glm::vec4(0.0, 1.0, 0.0, 0.0)); //Top Normals
		}
		return;
	}
	//Level != 0
	int spongeCount = 0;
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			for(int k=0; k<3; k++)
			{
				if(!(i == 1 && j == 1) && !(i== 1 && k ==1) && !(j == 1 && k ==1) && !(i == 1 && j == 1 && k == 1))
				{
					drawSponge(obj_vertices, obj_faces, vtx_normals, level-1,
						xvals[i], yvals[j], zvals[k],
						xvals[i+1], yvals[j+1], zvals[k+1]);
				}	
			}
		}
	}
}
