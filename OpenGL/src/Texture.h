#pragma once

#include "Renderer.h"

class Texture
{
private:
	GLuint m_RendererID;
	std::string m_Filepath;

	// local storage for the texture
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(GLuint slot=0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};