#include <iostream>
#include "chip8.h"
#include "graphics.h"
#include "input.h"
#include "sound.h"

int main()
{
	Chip8& chip8 = Chip8::singleton();
	Graphics& graphics = Graphics::singleton();
	Input& input = Input::singleton();

	chip8.init();

	char rom[15] = "roms/pong.rom";
	chip8.loadGame(rom);

	std::function<void()> cycle = std::bind(&Chip8::processCycle, &chip8);
	graphics.startLoop(cycle);

	return 0;
}