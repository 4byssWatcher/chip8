#include "chip8.h"
#include "graphics.h"
#include "input.h"
#include <random>

void Chip8::init()
{
	// Initialize registers and memory once
}

bool Chip8::drawFlag()
{
	// return drawFlag
}

void Chip8::emulateCycle()
{
	fetchOpcode();
	executeOpcode();
	updateTimers();
}

void Chip8::fetchOpcode()
{
	opcode = memory[PC] << 8 | memory[PC + 1]; // big endian
}

void Chip8::executeOpcode()
{
	unsigned short x = (opcode & 0x0F00) >> 8;
	unsigned short y = (opcode & 0x00F0) >> 4;
	unsigned short n = opcode & 0x000F;
	unsigned short nn = opcode & 0x00FF;
	unsigned short nnn = opcode & 0x0FFF;

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x0F00)
		{
		case 0x0000:
			if (opcode & 0x000F == 0x0000) // 00E0, clear the screen
			{
				Graphics* graphics = Graphics::singleton();
				graphics->displayClear();
			}
			else // 00EE, return
			{
				PC = stack[--SP];
			}
			break;

		default: // 0NNN, call
			stack[SP++] = PC;
			PC = nnn;
		}
		break;

	case 0x1000: // 1NNN, goto
		PC = nnn;
		break;

	case 0x2000: // 2NNN, call
		stack[SP++] = PC;
		PC = nnn;
		break;

	case 0x3000: // 3XNN, cond ==
		if (V[x] == nn) PC += 2;
		break;

	case 0x4000: // 4XNN, cond !=
		if (V[x] != nn) PC += 2;
		break;

	case 0x5000: // 5XY0, cond ==
		if (V[x] == V[y]) PC += 2;
		break;

	case 0x6000: // 6XNN, set
		V[x] = nn;
		break;

	case 0x7000: // 7XNN, add
		V[x] += nn;
		break;

	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 8XY0, assign
			V[x] = V[y];
			break;
		case 0x0001: // 8XY1, bitop
			V[x] |= V[y];
			break;
		case 0x0002: // 8XY2, bitop
			V[x] &= V[y];
			break;
		case 0x0003: // 8XY3, bitop
			V[x] ^= V[y];
			break;
		case 0x0004: // 8XY4, math
			V[x] += V[y];
			break;
		case 0x0005: // 8XY5, math
			V[x] -= V[y];
			break;
		case 0x0006: // 8XY6, bitop
			V[x] >>= 1;
			break;
		case 0x0007: // 8XY7, math
			V[x] = V[y] - V[x];
			break;
		case 0x000E: // 8XYE, bitop
			V[x] <<= 1;
			break;
		}
		break;

	case 0x9000: // 9XY0, cond
		if (V[x] != V[y]) PC += 2;
		break;

	case 0xA000: // ANNN, mem
		I = nnn;
		break;

	case 0xB000: // BNNN, flow
		PC = V[0] + nnn;
		break;

	case 0xC000: // CXNN, rand
		V[x] = (rand() % 255) & nn;
		break;

	case 0xD000: // DXYN, disp
		Graphics* graphics = Graphics::singleton();
		V[15] = graphics->drawSprite(x, y, 8, n + 1, memory + I);
		break;

	case 0xE000:
		Input * input = Input::singleton();
		switch (opcode & 0x000F)
		{
		case 0x000E: // EX9E, keyop
			if (input->isPressed(V[x]))PC += 2;
			break;
		case 0x0001: // EXA1, keyop
			if (!(input->isPressed(V[x])))PC += 2;
			break;
		}

	case 0xF000:
		switch (opcode & 0x000F)
		{
		case 0x0007: // FX07, timer

		}
	}
}