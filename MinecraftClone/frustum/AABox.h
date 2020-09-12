#pragma once
#include <glm/glm.hpp>

class AABox 
{

public:
	AABox(glm::vec3 corner, float x, float y, float z);
	AABox(void);
	~AABox();
	
	glm::vec3 corner;
	float x, y, z;

	void setBox(glm::vec3 corner, float x, float y, float z);

	// for use in frustum computations
	glm::vec3 getVertexP(glm::vec3 &normal);
	glm::vec3 getVertexN(glm::vec3 &normal);
};
