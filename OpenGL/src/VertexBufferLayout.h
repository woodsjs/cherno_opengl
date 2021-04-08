#pragma once
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
	GLuint type;
	GLuint count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(GLuint type)
	{
		switch (type)
		{
			case GL_FLOAT:
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;

	GLuint m_Stride;
public:
	//VertexBufferLayout();

	template<typename T>
	void Push(GLuint count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(GLuint count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<GLuint>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);

	}

	template<>
	void Push<unsigned char>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);

	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline GLuint GetStride() const { return m_Stride; }
};