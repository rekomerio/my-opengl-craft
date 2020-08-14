#pragma once
#include "GameEngineBase.h"
#include "Block.h"
#include <iostream>
#include <array>

class MinecraftEngine : public GameEngineBase
{
public:
	~MinecraftEngine();

	static MinecraftEngine* GetInstance() 
	{
		static MinecraftEngine* instance = new MinecraftEngine();
		return instance;
	}

private:
	MinecraftEngine();
	bool OnCreate();
	void Render(float elapsed);
	void Update(float elapsed);
	void OnClick(GLFWwindow* window, int button, int action, int mods);
	static void m_OnClick(GLFWwindow* window, int button, int action, int mods);

	GameObject* rootObject;
	GLuint shaderId;
};

