#pragma once

#include <string>
#include "GL/glew.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	GLuint m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);

	~Shader();

	void Bind()const;

	void Unbind() const;

	// set uniforms
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	bool CompileShader();
	int GetUniformLocation(const std::string& name);

	GLuint CompileShader(const std::string& source, GLuint type);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource ParseShader(const std::string& filePath);
};