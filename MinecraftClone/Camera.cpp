#include "Camera.h"

Camera::Camera()
{
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Fov = 90.0f;

	m_AspectRatio = 800.0f / 600.0f;

	m_MinDistance = 0.1f;
	m_MaxDistance = 100.0f;

	UpdateCameraFront();
	UpdateProjection();
}

void Camera::ApplyToProgram(GLuint shaderId)
{
	GLuint viewLocation = glGetUniformLocation(shaderId, "view");
	GLuint projectionLocation = glGetUniformLocation(shaderId, "projection");

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_View));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_Projection));
}

void Camera::SetYaw(float deg)
{
	m_Yaw = glm::mod(deg, 360.0f);
	UpdateCameraFront();
}

void Camera::SetPitch(float deg)
{
	m_Pitch = fmin(fmax(-89.9f, deg), 89.9f);
	UpdateCameraFront();
}

void Camera::AddYaw(float deg)
{
	SetYaw(m_Yaw + deg);
}

void Camera::AddPitch(float deg)
{
	SetPitch(m_Pitch + deg);
}

void Camera::SetAspectRatio(float ratio)
{
	m_AspectRatio = ratio;
	UpdateProjection();
}

void Camera::SetFov(float fov)
{
	m_Fov = fov;
	UpdateProjection();
}

void Camera::SetPosition(glm::vec3 position)
{
	m_Position = position;
	m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::MoveRelativeToDirection(float forward, float up, float left)
{
	m_Position += m_Front * forward;
	m_Position += m_Up * up;
	m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * left;

	m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::UpdateCameraFront()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(direction);
	m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::UpdateProjection()
{
	m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_MinDistance, m_MaxDistance);
}