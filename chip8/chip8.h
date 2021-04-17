#pragma once

#include <string>

struct Chip8
{

//public:
//	typedef void (Chip8::*Instruction)(unsigned short);

private:
	static const size_t MAX_GAME_SIZE = 0x1000 - 0x200;
	static const int FREQUENCY = 60;

	unsigned short opcode;
	unsigned char memory[4096];
	const unsigned char fontset[80] = "temp";
	unsigned char spriteAddr[16];

	unsigned char V[16]; // Resgiters V0-VE
	unsigned short I; // Index Register
	unsigned short PC; // Program Counter

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short SP; // Stack Pointer

	//Instruction instruction = nullptr;

	void fetchOpcode();
	void executeOpcode();
	void updateTimers();
	// delete?
	unsigned char getDelay() { return delay_timer; }
	void setDelay(unsigned char c) { delay_timer = c; }
	void setSound(unsigned char c) { sound_timer = c; }

	void regDump(unsigned char ed, unsigned char des);
	void regLoad(unsigned char ed, unsigned char des);

public:
	void init();
	void emulateCycle();
	void loadGame(std::string name);

	static Chip8& singleton();

	int getOpcode() { return opcode; }

	void processCycle();
	
};