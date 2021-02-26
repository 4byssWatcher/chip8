#pragma once

struct Chip8
{
private:
	unsigned short opcode;
	unsigned char memory[4096];

	unsigned char V[16]; // Resgiters V0-VE
	unsigned short I; // Index Register
	unsigned short PC; // Program Counter

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short SP; // Stack Pointer

	void fetchOpcode();
	void executeOpcode();
	void updateTimers();

public:
	void init();
	void emulateCycle();
	bool drawFlag();
	static Chip8* singleton();
	
};