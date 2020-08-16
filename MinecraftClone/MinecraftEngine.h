#pragma once
#include "GameEngineBase.h"
#include "Block.h"
#include <iostream>
#include <array>
#include "Camera.h"

class MinecraftEngine : public GameEngineBase
{
public:
	~MinecraftEngine();

	static MinecraftEngine* GetInstance();

private:
	MinecraftEngine();
	bool OnCreate();
	void Render(float elapsed);
	void Update(float elapsed);
	
	void OnClick(GLFWwindow* window, int button, int action, int mods);
	static void m_OnClick(GLFWwindow* window, int button, int action, int mods);
	void OnMouseMove(GLFWwindow* window, double x, double y);
	static void m_OnMouseMove(GLFWwindow* window, double x, double y);

	GameObject* rootObject;
	GLuint shaderId;
	Camera camera;
};

