#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "frustum/Plane.h"

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
	void SetPosition(glm::vec3 position);
	void MoveRelativeToDirection(float forward, float up, float left);
	inline float GetYaw() const { return m_Yaw; }
	inline float GetPitch() const { return m_Pitch; }
	inline float GetFov() const { return m_Fov; }
	inline glm::vec3 GetPosition() const { return m_Position; }
	bool IsPointInView(const glm::vec3& point) const;
	bool IsBoxInView(const glm::vec3& corner, const glm::vec3& size);
	/// <summary>
	/// This function must be called when camera has been 
	/// rotated or moved and culling is going to be done
	/// </summary>
	void PrepareForCulling();

	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;

private:
	void UpdateCameraFront();
	void UpdateProjection();

	float m_Pitch, m_Yaw;
	float m_Fov;
	float m_AspectRatio;
	float m_MinDistance, m_MaxDistance;


	glm::mat4 m_Projection;
	glm::mat4 m_View;
	
	// Culling stuff
	float m_Tang;
	float m_Nw, m_Nh, m_Fw, m_Fh;
	glm::vec3 m_Ntl, m_Ntr, m_Nbl, m_Nbr, m_Ftl, m_Ftr, m_Fbl, m_Fbr;
	Plane m_Planes[6];

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};
};

