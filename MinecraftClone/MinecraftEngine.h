#pragma once
#include "GameEngineBase.h"
#include "Block.h"
#include <iostream>
#include <array>

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
	/// <summary>
	/// Calls OnClick by accuiring engine instance
	/// </summary>
	static void m_OnClick(GLFWwindow* window, int button, int action, int mods);

	GameObject* rootObject;
	GLuint shaderId;
};

