#include "MinecraftEngine.h"

int main()
{
	//MinecraftEngine* engine = new MinecraftEngine();
	//engine->Run(800, 600, "Minecraft Clone");
	//delete engine;

	MinecraftEngine::GetInstance()->Run(800, 600, "Minecraft Clone");
	delete MinecraftEngine::GetInstance();

	return 0;
}