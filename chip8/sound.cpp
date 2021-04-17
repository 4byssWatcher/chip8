#include "sound.h"
#include <Windows.h>

Sound& Sound::singleton()
{
	static Sound instance;
	return instance;
}

bool Sound::playSound(std::string name)
{
	return PlaySoundA(name.c_str(), NULL, SND_FILENAME);
}