#pragma once

#include "glad\glad.h"
#include "GLFW\glfw3.h"

struct Input
{
private:
	static const int KEY_SIZE = 16;
	static constexpr const char* key = "0123456789ABCDEF";

	GLFWwindow* &window;

	Input(GLFWwindow*& window_) :window(window_) {}

public:
	bool isPressed(unsigned char key);
	unsigned char getKey();
	static Input& singleton();
};