#include "pch.h"
#include "../chip8/sound.h"

TEST(TestForSound, playSoundTest)
{
	Sound& sound = Sound::singleton();
	EXPECT_TRUE(sound.playSound("beep.wav"));
}