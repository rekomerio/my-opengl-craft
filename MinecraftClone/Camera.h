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
	void SetAspectRatio(float ratio);
	void SetFov(float fov);
	void SetPosition(float x, float y, float z);
	void MoveRelativeToDirection(float forward, float up, float left);
	inline float GetYaw() const { return m_Yaw; }
	inline float GetPitch() const { return m_Pitch; }
	inline glm::vec3 GetPosition() const { return cameraPosition; }

private:
	void UpdateCameraFront();
	void UpdateProjection();

	float m_Pitch, m_Yaw;
	float m_Fov;
	float m_AspectRatio;
	float m_MinDistance, m_MaxDistance;

	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 projection;
	glm::mat4 view;
};

