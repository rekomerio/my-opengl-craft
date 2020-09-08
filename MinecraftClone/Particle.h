#pragma once
#include "GameModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle : public GameModel
{
public:
	Particle();
	void Update(float elapsed);

	float lifeSpan, lifeLeft;
	bool isAlive;

	glm::vec4 color;
	glm::vec3 velocity;

	friend class ParticleHandler;
};

