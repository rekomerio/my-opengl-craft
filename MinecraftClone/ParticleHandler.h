#pragma once
#include "Particle.h"
#include "Camera.h"
#include <glm/glm.hpp>

class ParticleHandler
{
public:
	ParticleHandler(size_t maxParticles);
	void Update(float elapsed);
	void Render(float elapsed, GLuint activeShader);
	void Emit(Particle particle);

	glm::vec3 position; 
	bool isActive; 
private:
	std::vector<Particle> particles;
	int particleIndex;
};

