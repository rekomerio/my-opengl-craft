#include "Particle.h"

Particle::Particle()
{
	isAlive = true;
	lifeSpan = 1.0f;
	lifeLeft = lifeSpan;
	color = glm::vec4(1.0f);
	velocity = glm::vec3(1.0f);
}

void Particle::Update(float elapsed)
{
	lifeLeft -= elapsed;
	
	if (lifeLeft < 0.0f)
	{
		isAlive = false;
		return;
	}

	color.w = lifeLeft / lifeSpan;

	GameModel::SetPosition(m_Position + (velocity * elapsed));
}