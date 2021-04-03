#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ( " << error << " )" << std::endl;
	}
}

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static GLuint CompileShader(const std::string& source, GLuint type)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// TODO: error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char)); 
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// we need a current context to call this, and we do. It's window
	// this sets vsync = number of frame updates to wait before buffers are swapped
	// less than zero immediately swaps, ready or not
	// 0 is fast
	// 1 stops tearing
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// This gets our buffer ready to draw
	// gets the data, buffer, and yeah.
	// each "line" represents a vertex attribute group
	// the vertex attribute group can have the vertex, vector, color, normals, etc
	// typically these are size of 1,2,3,4
	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// buffer size
	GLuint sizeOfBuffer = *(&positions + 1) - positions;
	glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer * sizeof(float), positions, GL_STATIC_DRAW);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// buffer size
	GLuint sizeOfIBOBuffer = *(&indicies + 1) - indicies;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIBOBuffer * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	GLuint shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);
	
	// using the color uniform
	int location = glGetUniformLocation(shader, "u_Color");
	assert(location >= 0);
	// 4 floats for the u_Color
	glUniform4f(location, 0.1f, 0.0f, 0.4f, 1.0f);

	// these will allow us to change the uniform color in flight
	float red = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, sizeOfBuffer/2);

		// set our uniform color 
		// this is u_color
		glUniform4f(location, red, 0.0f, 0.4f, 1.0f);

		// the big daddy of drawing!!!!
		GLClearError();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		GLCheckError();

		// now that it's drawn, we can change the color
		if (red > 1.0f)
			increment = -0.05f;
		else if (red < 0.0f)
			increment = 0.05f;

		red += increment;

		/* Swap front and back buffers */
		// here our color was changed and sitting in the back buffer
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}