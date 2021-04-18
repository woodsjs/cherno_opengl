#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Shader.h"
#include "Texture.h"

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

	// we need a current context to call this, and we do have one. It's window
	// this sets vsync = number of frame updates to wait before buffers are swapped
	// less than zero immediately swaps, ready or not
	// 0 is fast
	// 1 stops tearing
	// we almost always want this to be 1
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Let's get our buffer ready to draw
	// We need to get the data, buffer, and yeah.
	// 
	// Positions is our array of vertex attributes
	// each "line" represents a vertex attribute group
	// the vertex attribute group can have the vertex, vector, color, normals, etc
	// typically these are size of 1,2,3, or 4
	// at first we had just our f2 locations of each pixel, now we add our next f2 for texture locations
	// since the texture starts at bottom left as 0.0f, 0.0f, we have to line that up with teh bottom left vertex
	// just the same, 0.5f, 0.5f is the top right, so that would be 1.0f, 1.0f
	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};

	// fix for alpha images
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// This will be used to tell the renderer to draw our 
	// vertices from positions array in this particular order 
	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint sizeOfBuffer = *(&positions + 1) - positions;

	// This is what it says, a buffer of vertices
	// Since this is built from our array of attributes it can include
	// position, color, etc
	// specifically, this is an array buffer
	VertexBuffer vb(positions, sizeOfBuffer * sizeof(float));

	// the buffer layout takes in our indicies array length
	// indicies are each drawn geometry
	// and stores, per line pushed, a set that includes
	// type (eg. int), count (here 2), and t/f
	// as well as the stride, so in the layout, what's the type size * count
	// this allows us to skip through the elements
	VertexBufferLayout layout;
	// this is for the vertex
	layout.Push<float>(2);
	// this is for the texture
	layout.Push<float>(2);

	// for each line in our layout pushed in, 
	// we're going to add the buffer and layout to the vertex array
	// this array is used later to actually draw what we want
	VertexArray va;
	va.AddBuffer(vb, layout);

	GLuint sizeOfIBOBuffer = *(&indicies + 1) - indicies;
	// we need the index buffer to know how to draw our vertex array attributes
	IndexBuffer ib(indicies, sizeOfIBOBuffer);

	va.Bind();

	// shader is our program that tells us what to do per pixel
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	shader.SetUniform4f("u_Color", 0.1f, 0.0f, 0.4f, 1.0f);

	// we're going to now grab a texture to work with. Here it's a simple PNG
	Texture texture("res/textures/amabrick.png");
	texture.Bind();
	// the int should be the same as texture.Bind.  Here we use default, zero
	// and yes, we pass the bound texture to the shader to render over the geometry
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	Renderer renderer;

	// these will allow us to change the uniform color in flight
	float red = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		//shader.Bind();
		shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

		// the big daddy of drawing!!!!
		GLClearError();
		renderer.Draw(va, ib, shader);
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