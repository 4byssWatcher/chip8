#include "chip8.h"
#include "graphics.h"
#include "input.h"
#include "sound.h"
#include "utils.h"
#include "clock.h"
#include <cstdio>
#include <random>

Chip8& Chip8::singleton()
{
	static Chip8 instance;
	return instance;
}

void Chip8::init()
{
	// Initialize registers and memory once
	PC	   = 0x200;
	opcode = 0;
	I	   = 0;
	SP	   = 0;

	memset(memory, 0, sizeof(memory));
	memset(stack, 0, sizeof(stack));
	memset(V, 0, sizeof(V));

	for (int i = 0; i < 80; ++i)
		memory[i] = fontset[i];
	
	delay_timer = 60;
	sound_timer = 60;
}

void Chip8::loadGame(std::string fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.c_str(), "rb");
	if (file == nullptr)
	{
		printf("rom not found.");
		exit(0);
	}
	int tmp = fread(memory + 0x200, 1, MAX_GAME_SIZE, file);

	fclose(file);
}

void Chip8::emulateCycle()
{
	fetchOpcode();
	executeOpcode();
	updateTimers();
}

void Chip8::fetchOpcode()
{
	opcode = (memory[PC] << 8) | memory[PC + 1]; // big endian
}

void Chip8::executeOpcode()
{
	/*
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch0(opcode);
		break;
	case 0x1000:
		instruction = &Chip8::jp;
		break;
	case 0x2000:
		instruction = &Chip8::call;
		break;
	case 0x3000:
		instruction = &Chip8::se;
		break;
	case 0x4000:
		instruction = &Chip8::sne;
		break;
	case 0x5000:
		instruction = &Chip8::sev;
		break;
	case 0x6000:
		instruction = &Chip8::ld;
		break;
	case 0x7000:
		instruction = &Chip8::add;
		break;
	case 0x8000:
		switch8(opcode);
		break;
	case 0x9000:
		instruction = &Chip8::snev;
		break;
	case 0xA000:
		instruction = &Chip8::ldI;
		break;
	case 0xB000:
		instruction = &Chip8::se;
		break;
	case 0xC000:
		instruction = &Chip8::se;
		break;
	case 0xD000:
		instruction = &Chip8::se;
		break;
	case 0xE000:
		instruction = &Chip8::se;
		break;
	case 0xF000:
		instruction = &Chip8::se;
		break;
	}
	*/

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
				Graphics& graphics = Graphics::singleton();
				graphics.displayClear();
				PC += 2;
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
		stack[SP++] = PC + 2;
		PC = nnn;
		break;

	case 0x3000: // 3XNN, cond ==
		if (V[x] == nn) PC += 2;
		PC += 2;
		break;

	case 0x4000: // 4XNN, cond !=
		if (V[x] != nn) PC += 2;
		PC += 2;
		break;

	case 0x5000: // 5XY0, cond ==
		if (V[x] == V[y]) PC += 2;
		PC += 2;
		break;

	case 0x6000: // 6XNN, set
		V[x] = nn;
		PC += 2;
		break;

	case 0x7000: // 7XNN, add
		V[x] += nn;
		PC += 2;
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
		PC += 2;
		break;

	case 0x9000: // 9XY0, cond
		if (V[x] != V[y]) PC += 2;
		PC += 2;
		break;

	case 0xA000: // ANNN, mem
		I = nnn;
		PC += 2;
		break;

	case 0xB000: // BNNN, flow
		PC = V[0] + nnn;
		PC += 2;
		break;

	case 0xC000: // CXNN, rand
		V[x] = (rand() % 255) & nn;
		PC += 2;
		break;

	case 0xD000: // DXYN, disp
	{ 
		V[0xF] = 0;
		Graphics& graphics = Graphics::singleton();
		V[0xF] = graphics.drawSprite(V[x], V[y], n, 8, memory + I);
		PC += 2;
		break;
	}

	case 0xE000:
	{
		Input& input = Input::singleton();
		switch (opcode & 0x000F)
		{
		case 0x000E: // EX9E, keyop
			if (input.isPressed(V[x]))PC += 2;
			break;
		case 0x0001: // EXA1, keyop
			if (!(input.isPressed(V[x])))PC += 2;
			break;
		}
		PC += 2;
	}

	case 0xF000:
		switch (opcode & 0x00F0)
		{
		case 0x0000:
			if (n == 0x0007) // FX07, timer
				V[x] = getDelay();
			else			 // FX0A, keyOp
			{
				Input& input = Input::singleton();
				V[x] = input.getKey();
			}
			break;
		case 0x0010:
			if (n == 0x0005)        // FX15, timer
				setDelay(V[x]);
			else if (n == 0x0008)   // FX18, sound
				setSound(V[x]);
			else					// FX1E, mem
				I += V[x];
			break;
		case 0x0020: // FX29, mem
			I = spriteAddr[V[x]];
			break;
		case 0x0030: // FX33, bcd
			BCD::setBCD(V[x]);
			memory[I + 0] = BCD::getBCD(3);
			memory[I + 1] = BCD::getBCD(2);
			memory[I + 2] = BCD::getBCD(1);
			break;
		case 0x0050: // FX55, mem
			regDump(x, I);
			break;
		case 0x0060: // FX65, mem
			regLoad(x, I);
			break;
		}
		PC += 2;
	}
}

void Chip8::regDump(unsigned char ed, unsigned char des)
{
	for (int i = 0; i <= ed; ++i)
		memory[des + i] = V[i];
	I += ed + 1;
}

void Chip8::regLoad(unsigned char ed, unsigned char des)
{
	for (int i = 0; i <= ed; ++i)
		V[i] = memory[des + i];
	I += ed + 1;
}

void Chip8::updateTimers()
{
	if (delay_timer > 0)
		--delay_timer;
	if (sound_timer > 0)
	{
		--sound_timer;
		if (sound_timer == 0)
		{
			Sound& sound = Sound::singleton();
			sound.playSound("sound/beep.wav");
		}
	}
}

void Chip8::processCycle()
{
	static Clock clock;

	int cycles = clock.getCycles(FREQUENCY);
	while (cycles--)
		emulateCycle();
}