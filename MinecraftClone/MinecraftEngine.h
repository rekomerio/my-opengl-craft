#pragma once
#include "GameEngineBase.h"
#include "Block.h"
#include "Player.h"
#include "CollisionHandler.h"
#include "CollisionBox.h"
#include <iostream>
#include <array>

class MinecraftEngine : public GameEngineBase
{
public:
	~MinecraftEngine();

	static MinecraftEngine* GetInstance();
	GLuint& GetActiveShader();

private:
	MinecraftEngine();
	bool OnCreate();
	void Render(float elapsed);
	void Update(float elapsed);
	
	void OnClick(GLFWwindow* window, int button, int action, int mods);
	void OnMouseMove(GLFWwindow* window, double x, double y);
	void OnResize(GLFWwindow* window, int width, int height);
	static void m_OnClick(GLFWwindow* window, int button, int action, int mods);
	static void m_OnMouseMove(GLFWwindow* window, double x, double y);
	static void m_OnResize(GLFWwindow* window, int width, int height);

	GameObject* rootObject;
	Player* player;
	GLuint activeShader;
	CollisionHandler collisionHandler;
};

