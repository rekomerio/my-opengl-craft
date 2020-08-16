#include "Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	fov = 90.0f;
}

void Camera::ApplyToProgram(GLuint shaderId)
{
	constexpr float width = 800.0f;
	constexpr float height = 600.0f;
	constexpr float ratio = width / height;
	constexpr float minDistance = 1.0f;
	constexpr float maxDistance = 100.0f;

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), ratio, minDistance, maxDistance);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	GLuint viewLoc = glGetUniformLocation(shaderId, "view");
	GLuint projectionLoc = glGetUniformLocation(shaderId, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::SetYaw(float deg)
{
	m_yaw = glm::mod(deg, 360.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::SetPitch(float deg)
{
	m_pitch = fmin(fmax(-89.0f, deg), 89.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::AddYaw(float deg)
{
	SetYaw(m_yaw + deg);
}

void Camera::AddPitch(float deg)
{
	SetPitch(m_pitch + deg);
}

inline float Camera::GetYaw()
{
	return m_yaw;
}

inline float Camera::GetPitch()
{
	return m_pitch;
}
