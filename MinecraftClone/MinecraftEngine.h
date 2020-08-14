#pragma once
#include "GameEngineBase.h"
#include "Cube.h"
#include <iostream>
#include <array>

class MinecraftEngine : public GameEngineBase
{
public:
	MinecraftEngine();
	~MinecraftEngine();

	bool OnCreate();
	void Render(float elapsed);
	void Update(float elapsed);

private:
	GameObject* rootObject;
	GLuint shaderId;
};

