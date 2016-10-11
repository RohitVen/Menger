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
	double minx=-0.5, miny=-0.5, minz=-0.5, maxx=0.5, maxy=0.5, maxz=0.5;
	if(nesting_level_==0)
	{
		drawCube(obj_vertices, obj_faces,
			minx, miny, minz,
			maxx, maxy, maxz);
	}
	else
	{
		drawSponge(obj_vertices, obj_faces,	nesting_level_,
		minx, miny, minz, 
		maxx, maxy, maxz);
	}
	// std::cout<<"\n\nDRAWING CUBE";
	//Loading vert normals into the VBO
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
	// std::cout<<"\nLoaded verts!!";
}

void Menger::drawCube(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces,
	double minx, double miny, double minz, 
	double maxx, double maxy, double maxz)
{
	typedef glm::vec4 point4;
	//Defining all the vertices
	point4 vertPos[8] = {
		point4(minx, miny, maxz, 1.0), //Back bottom left
		point4(minx, maxy, maxz, 1.0), //Back top left
		point4(maxx, maxy, maxz, 1.0), //Back top right
		point4(maxx, miny, maxz, 1.0), //Back bottom right
		point4(minx, miny, minz, 1.0), //Front bottom left 
		point4(minx, maxy, minz, 1.0), //Front top left
		point4(maxx, maxy, minz, 1.0), //Front top right
		point4(maxx, miny, minz, 1.0)  //Front bottom right
	};
	// std::cout<<"\nabout to load verts";
	//Loading the vertices into the VBO
	obj_vertices.push_back(vertPos[0]);
	obj_vertices.push_back(vertPos[1]);
	obj_vertices.push_back(vertPos[2]);
	obj_vertices.push_back(vertPos[3]);
	obj_vertices.push_back(vertPos[4]);
	obj_vertices.push_back(vertPos[5]);
	obj_vertices.push_back(vertPos[6]);
	obj_vertices.push_back(vertPos[7]);
	//Loading faces into the VBO
	obj_faces.push_back(glm::uvec3(0, 1, 3)); //Back face
	obj_faces.push_back(glm::uvec3(1, 2, 3)); //Back face
	obj_faces.push_back(glm::uvec3(2, 3, 7)); //Right face
	obj_faces.push_back(glm::uvec3(7, 6, 2)); //Right face
	obj_faces.push_back(glm::uvec3(3, 0, 4)); //Bottom face
	obj_faces.push_back(glm::uvec3(4, 3, 7)); //Bottom face
	obj_faces.push_back(glm::uvec3(4, 5, 6)); //Front face
	obj_faces.push_back(glm::uvec3(4, 6, 7)); //Front face
	obj_faces.push_back(glm::uvec3(5, 4, 0)); //Left face
	obj_faces.push_back(glm::uvec3(5, 0, 1)); //Left face
	obj_faces.push_back(glm::uvec3(6, 5, 1)); //Top face
	obj_faces.push_back(glm::uvec3(5, 1, 2)); //Top face
}

void Menger::drawSponge(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces, 
	int level,
	double minx, double miny, double minz, 
	double maxx, double maxy, double maxz)
{
	// std::cout<<"\ndrawing the sponge!!";
	double xvals[4] = {minx, minx/3, maxx/3, maxx};
	double yvals[4] = {miny, miny/3, maxy/3, maxy};
	double zvals[4] = {minz, minz/3, maxz/3, maxz};

	//Base case
	if(level == 1)
	{

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				for(int k=0; k<3; k++)
				{
					int front = (i==1 && j==1 && k==0); //Front center
					int back = (i==1 && j==1 && k==2); //Back center
					int right = (i==2 && j==1 && k==1); //Right center
					int left = (i==0 && j==1 && k==1); //Left center
					int top = (i==1 && j==2 && k==1); //Top center
					int bottom = (i==1 && j==0 && k==1); //Bottom center
					if(!front && !back && !right && !left && !top && !bottom) //Front center
					{
						// std::cout<<"\nDrawing a cube!";
						// std::cout<<"\ni j k:"<<i<<" "<<j<<" "<<k;
						drawCube(obj_vertices, obj_faces,
							xvals[i], yvals[j], zvals[k],
							xvals[i+1], yvals[j+1], zvals[k+1]);
					}	
				}
			}
		}
		return;
	}
	//Level != 1
	for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				for(int k=0; k<3; k++)
				{
					int front = (i==1 && j==1 && k==0); //Front center
					int back = (i==1 && j==1 && k==2); //Back center
					int right = (i==2 && j==1 && k==1); //Right center
					int left = (i==0 && j==1 && k==1); //Left center
					int top = (i==1 && j==2 && k==1); //Top center
					int bottom = (i==1 && j==0 && k==1); //Bottom center
					if(!front && !back && !right && !left && !top && !bottom) //Front center
					{
						// std::cout<<"\nDrawing a cube!";
						// std::cout<<"\ni j k:"<<i<<" "<<j<<" "<<k;
						drawSponge(obj_vertices, obj_faces, level-1,
							xvals[i], yvals[j], zvals[k],
							xvals[i+1], yvals[j+1], zvals[k+1]);
					}	
				}
			}
		}

}




// typedef glm::vec4 color4;
		// //Color definitions
		// color4 vertColors[8] = {
		// 	color4(0.0, 0.0, 0.0, 1.0), //Black
		// 	color4(1.0, 0.0, 0.0, 1.0), //Red
		// 	color4(1.0, 1.0, 0.0, 1.0), //Yellow
		// 	color4(0.0, 1.0, 0.0, 1.0), //Green
		// 	color4(0.0, 0.0, 1.0, 1.0), //Blue
		// 	color4(1.0, 0.0, 1.0, 1.0), //Magenta
		// 	color4(1.0, 1.0, 1.0, 1.0), //White
		// 	color4(0.0, 1.0, 1.0, 1.0), //Cyan
		// };

