#include <iostream>
#include "chip8.h"
#include "graphics.h"
#include "input.h"

int main()
{
	Chip8 chip8;
	Graphics graphics;
	Input input;

	graphics.init();
	input.init();
	chip8.init();
	chip8.loadGame("pong");

	while (true)
	{
		chip8.emulateCycle();

		if (chip8.drawFlag())
			graphics.draw();

		input.setKeys();
	}
	return 0;
}