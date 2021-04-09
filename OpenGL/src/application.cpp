#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	VertexArray va;
	GLuint sizeOfBuffer = *(&positions + 1) - positions;
	VertexBuffer vb(positions, sizeOfBuffer * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	GLuint sizeOfIBOBuffer = *(&indicies + 1) - indicies;
	IndexBuffer ib(indicies, sizeOfIBOBuffer);

	va.Bind();

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	shader.SetUniform4f("u_Color", 0.1f, 0.0f, 0.4f, 1.0f);

	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	// these will allow us to change the uniform color in flight
	float red = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

		va.Bind();
		ib.Bind();

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

	glfwTerminate();
	return 0;
}