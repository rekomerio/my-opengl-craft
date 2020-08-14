#include "MinecraftEngine.h"

template <int W, int H>
struct MinecraftEngineSingleton {
	void Run() 
	{
		MinecraftEngine::GetInstance()->Run(W, H, "Minecraft Engine");
		delete MinecraftEngine::GetInstance();
	}
};

int main()
{
	MinecraftEngineSingleton<800, 600> engine;
	engine.Run();

	return 0;
}