#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ( " << error << " )" << std::endl;
	}
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	//shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

