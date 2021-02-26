#pragma once

struct Input
{
private:
	unsigned char key[16];

public:
	void init();
	bool isPressed(unsigned char key);
	static Input* singleton();
};