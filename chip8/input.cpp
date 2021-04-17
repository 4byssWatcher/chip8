#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "graphics.h"
#include "input.h"

// 0-9, A-F, ASCII
bool Input::isPressed(unsigned char id)
{
	return glfwGetKey(window, key[id]) == GLFW_PRESS;
}

unsigned char Input::getKey()
{
	for (int i = 0; i < KEY_SIZE; ++i)
	{
		if (isPressed(i))
			return i;
	}
}

Input& Input::singleton()
{
	Graphics& graphics = Graphics::singleton();
	GLFWwindow*& window_ = graphics.getWindow();
	static Input instance(window_);
	return instance;
}