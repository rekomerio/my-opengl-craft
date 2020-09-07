#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle : public GameObject
{
public:
	Particle();
	void Update(float elapsed) override;
	void Render(float elapsed, GLuint activeShader);

	bool isAlive;
	float lifeSpan;
	float elapsedLife;

	glm::vec4 color;
	glm::vec3 velocity;
	Mesh* mesh;
};

