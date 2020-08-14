#pragma once
#include <string>

class BlockBase
{
private:
	int size;
	int x, y;
	int texture;
	float strength;
	std::string audioSourceOnTouch;
	std::string audioSourceOnWalk;
};

