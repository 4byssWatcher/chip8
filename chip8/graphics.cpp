#include <cstring>
#include <iostream>

#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "graphics.h"


const char* v_shader_src =
"#version 100\n"
"in vec2 coords;\n"
"varying vec2 tex_coords;\n"
"void main(){\n"
"	gl_Position = vec4(coords.x, coords.y, 0.0, 1.0);\n"
"	tex_coords.x = (1.0 + coords.x)/2.0;\n"
"	tex_coords.y = (1.0 - coords.y)/2.0;\n"
"}";

const char* f_shader_src =
"#version 100\n"
"in lowp vec2 tex_coords;\n"
"uniform sampler2D tex;\n"
"void main(){\n"
"	gl_FragColor = vec4(vec3(texture2D(tex, tex_coords).a), 1.0);\n"
"}";

const float vertices[] =
{
	-1.0f, -1.0f,
	-1.0f, +1.0f,
	+1.0f, -1.0f,
	+1.0f, +1.0f,
};

void check_shader_log(unsigned shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
		puts("Shader compile log:");
		puts(log);
	}
}


void Graphics::displayClear()
{
	memset(pixels, 0, sizeof(pixels));
}

Graphics& Graphics::singleton()
{
	static Graphics instance;
	return instance;
}

void setup_shaders()
{
	/* Create vertex shader */
	unsigned v_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_shader, 1, &v_shader_src, NULL);
	glCompileShader(v_shader);
	check_shader_log(v_shader);

	/* Create fragment shader */
	unsigned f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_shader, 1, &f_shader_src, NULL);
	glCompileShader(f_shader);
	check_shader_log(f_shader);

	/* Create shader program */
	unsigned program = glCreateProgram();
	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	glLinkProgram(program);
	glUseProgram(program);

	/* Remove shaders */
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	/* Create vertex buffer */
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Setup vertex input array */
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Graphics::setupTexture()
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ROWS, COLS, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
}


bool Graphics::drawSprite(int x, int y, int w, int h, unsigned char* st)
{
	//std::cout << "drawsprite" << std::endl;

	bool res = false;
	for (int i = 0; i < w; ++i)
	{
		unsigned short pixel = st[i];
		for (int j = 0; j < h; ++j)
		{
			if (pixel & (0x80 >> i))
			{
				if (pixels[((x + i) << 6) + y + j])res = 1;
				pixels[((x + i) << 6) + y + j] ^= 1;
			}
		}
	}
	drawFlag = true;
	return res;
}

Graphics::Graphics()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "chip8", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSwapInterval(1);

	setupTexture();
	setup_shaders();

	drawFlag = true;
	displayClear();
}

bool Graphics::check()
{
	static bool in = false;
	if (!in)
	{
		for (int i = 0; i < ROWS; ++i)
		{
			for (int j = 0; j < COLS; ++j)
				printf("%d", pixels[i * ROWS + j]);
				printf("\n");
		}
		in = true;
	}
	
	return true;
}

void Graphics::startLoop(std::function<void()> cycle)
{
	glClear(GL_COLOR_BUFFER_BIT);

	while (!glfwWindowShouldClose(window))
	{
		cycle();
		refresh();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Graphics::refresh()
{
	if (!drawFlag)return;

	//if (!check())printf("!!\n");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ROWS, COLS, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glfwSwapBuffers(window);
	glfwPollEvents();
}