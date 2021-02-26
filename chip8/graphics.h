#pragma once

struct Graphics
{
private:
	unsigned char pixel[64 * 32];

public:
	void init();
	void draw();

	// return 1 if any screen pixels are flipped from set to unset
	bool drawSprite(int x, int y, int w, int h, unsigned char* st);

	void displayClear();
	static Graphics* singleton();
};