#include "ParticleHandler.h"

ParticleHandler::ParticleHandler(size_t maxParticles)
{
	particles.resize(maxParticles);
	particleIndex = 0;
	position = glm::vec3(0.0f);
	isActive = true;

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i] = Particle();
		particles[i].isAlive = false;
	}
}

void ParticleHandler::Update(float elapsed)
{
	if (!isActive) return;

	for (auto& particle : particles)
	{
		if (particle.isAlive)
			particle.Update(elapsed);
	}
}

void ParticleHandler::Render(float elapsed, GLuint activeShader)
{
	if (!isActive) return;

	for (auto& particle : particles)
	{
		if (particle.isAlive)
			particle.Render(elapsed, activeShader);
	}
}

void ParticleHandler::Emit(Particle particle)
{
	particle.SetPosition(ParticleHandler::position);
	particles[particleIndex] = particle;
	++particleIndex %= particles.size();
}
