#include "Camera.h"

Camera::Camera()
{
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Fov = 90.0f;

	m_AspectRatio = 800.0f / 600.0f;

	m_MinDistance = 0.5f;
	m_MaxDistance = 80.0f;

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

	// Culling stuff
	m_Tang = (float)tan(glm::radians(m_Fov) * 0.5);
	m_Nh = m_MinDistance * m_Tang;
	m_Nw = m_Nh * m_AspectRatio;
	m_Fh = m_MaxDistance * m_Tang;
	m_Fw = m_Fh * m_AspectRatio;
}

bool Camera::IsPointInView(const glm::vec3& point) const
{
	for (size_t i = 0; i < 6; i++)
	{
		if (m_Planes[i].DistanceTo(point) < 0)
			return false;
	}
	return true;
}

// Kinda works, but there are situations where none of the corners are in view but the box still is
bool Camera::IsBoxInView(const glm::vec3& corner, const glm::vec3& size) const
{
	for (size_t x = 0; x < 2; x++)
		for (size_t y = 0; y < 2; y++)	
			for (size_t z = 0; z < 2; z++)
				if (IsPointInView(corner + (glm::vec3(x, y, z) * size)))
					return true;

	return false;
}

void Camera::PrepareForCulling()
{
	glm::vec3 nc, fc, x, y, z;

	z = m_Position - (m_Position + m_Front);
	z = glm::normalize(z);

	x = glm::cross(m_Up, z);
	x = glm::normalize(x);

	y = glm::cross(z, x);

	nc = m_Position - z * m_MinDistance;
	fc = m_Position - z * m_MaxDistance;

	m_Ntl = nc + y * m_Nh - x * m_Nw;
	m_Ntr = nc + y * m_Nh + x * m_Nw;
	m_Nbl = nc - y * m_Nh - x * m_Nw;
	m_Nbr = nc - y * m_Nh + x * m_Nw;

	m_Ftl = fc + y * m_Fh - x * m_Fw;
	m_Ftr = fc + y * m_Fh + x * m_Fw;
	m_Fbl = fc - y * m_Fh - x * m_Fw;
	m_Fbr = fc - y * m_Fh + x * m_Fw;

	m_Planes[TOP].Set3Points(m_Ntr, m_Ntl, m_Ftl);
	m_Planes[BOTTOM].Set3Points(m_Nbl, m_Nbr, m_Fbr);
	m_Planes[LEFT].Set3Points(m_Ntl, m_Nbl, m_Fbl);
	m_Planes[RIGHT].Set3Points(m_Nbr, m_Ntr, m_Fbr);
	m_Planes[NEARP].Set3Points(m_Ntl, m_Ntr, m_Nbr);
	m_Planes[FARP].Set3Points(m_Ftr, m_Ftl, m_Fbl);
}
