#ifndef GL_SHADER_H
#define GL_SHADER_H

#pragma once

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <filesystem>
#include <fstream>
#include <string>


/// <summary>
/// Wrapper for OpenGL shader 
/// </summary>
class GlShader
{
public:
	// constructor generates the shader on the fly
	GlShader(const std::string& vertexPath, const std::string& fragmentPath);

	// activate the shader
	void Active();

	void Delete();

	// ------------------------------------------------------------------------
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;
	
	void setFloat(const std::string& name, float value) const;
	
	const unsigned int GetId() const;
private:
	unsigned int ID;
	// utility function for checking shader compilation/linking errors.
	void checkCompileErrors(unsigned int shader, std::string type);

	std::string ReadShaderFile(const std::string fileName);
};

#endif