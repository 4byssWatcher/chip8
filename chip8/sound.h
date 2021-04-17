#pragma once

#include <string>

struct Sound
{
public:
	static Sound& singleton();
	bool playSound(std::string name);
};