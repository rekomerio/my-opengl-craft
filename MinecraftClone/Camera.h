#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();

	void ApplyToProgram(GLuint shaderId);
	void SetYaw(float deg);
	void SetPitch(float deg);
	void AddYaw(float deg);
	void AddPitch(float deg);
	float GetYaw();
	float GetPitch();

private:
	float x, y, z;
	float m_pitch, m_yaw;
	float fov;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

