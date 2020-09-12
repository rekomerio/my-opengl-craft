#pragma once
#include <glm/glm.hpp>

class Plane  
{
public:
	Plane(glm::vec3 v1,  glm::vec3 v2,  glm::vec3 v3);
	Plane();
	~Plane();

	void Set3Points( glm::vec3 v1,  glm::vec3 v2,  glm::vec3 v3);
	void SetNormalAndPoint(glm::vec3 normal, glm::vec3 point);
	void SetCoefficients(float a, float b, float c, float d);
	float DistanceTo(const glm::vec3& point) const;

	glm::vec3 normal, point;
	float d;
};