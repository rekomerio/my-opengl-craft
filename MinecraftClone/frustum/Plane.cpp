#include "Plane.h"

Plane::Plane() 
{
	normal = glm::vec3(0.0f);
	d = 0.0f;
}

Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) 
{
	Set3Points(v1, v2, v3);
}

Plane::~Plane() {}

void Plane::Set3Points(glm::vec3 v1,  glm::vec3 v2,  glm::vec3 v3) 
{
	normal = glm::cross((v3 - v2), (v1 - v2));
	normal = glm::normalize(normal);

	point = v2;
	d = -glm::dot(normal, point);
}

void Plane::SetNormalAndPoint(glm::vec3 normal, glm::vec3 point) 
{
	this->normal = glm::normalize(normal);
	d = -glm::dot(this->normal, point);
}

void Plane::SetCoefficients(float a, float b, float c, float d) 
{
	// set the normal vector
	normal = glm::vec3(a, b, c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal = glm::vec3(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d / l;
}

float Plane::DistanceTo(const glm::vec3& point) const
{
	return (d + glm::dot(normal, point));
}

