#pragma once

#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include <functional>

struct Graphics
{
private:
	static const int ROWS = 64;
	static const int COLS = 32;

	constexpr static const int SCREEN_WIDTH  = ROWS * 10;
	constexpr static const int SCREEN_HEIGHT = COLS * 10;

	bool drawFlag;

	unsigned char pixels[ROWS * COLS];
	GLFWwindow* window;

	void setupTexture();
	bool check();

public:
	Graphics();

	GLFWwindow*& getWindow() { return window; }

	void refresh();

	// return 1 if any screen pixels are flipped from set to unset
	bool drawSprite(int x, int y, int w, int h, unsigned char* st);

	void displayClear();
	static Graphics& singleton();

	void startLoop(std::function<void()> cycle);
};