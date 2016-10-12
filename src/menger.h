#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>

class Menger {
public:
	Menger();
	~Menger();
	void set_nesting_level(int);
	bool is_dirty() const;
	void set_clean();
	void generate_geometry(std::vector<glm::vec4>& obj_vertices,
			       std::vector<glm::vec4>& vtx_normals,
	                       std::vector<glm::uvec3>& obj_faces);

	void drawSponge(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces, std::vector<glm::vec4>& vtx_normals, 
		int level, double minx, double miny, double minz, double maxx, double maxy, double maxz);

	void drawCube(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces,
	double minx, double miny, double minz, double maxx, double maxy, double maxz);

private:
	bool dirty_ = false;

public:
	int numVerts = 36;
	typedef glm::vec4 point4;
	typedef glm::vec4 color4;
	point4 vertPos[];
	color4 vertColors[];
	int index;
	int nesting_level_ = 0;
};

#endif
