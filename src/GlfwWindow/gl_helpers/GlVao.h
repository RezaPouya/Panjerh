#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#pragma once
#include <glad/gl.h>
#include <vector>
#include <stdexcept>
#include "GlVbo.h"

/// <summary>
/// Wrapper for Vertex Array Object 
/// </summary>
class GlVao {
private:
	GLuint m_ID;
	bool m_IsBound;

public:
	// Constructor & Destructor
	GlVao(bool shouldBind = true);
	~GlVao();

	// Delete copy operations
	GlVao(const GlVao&) = delete;
	GlVao& operator=(const GlVao&) = delete;

	// Move operations
	GlVao(GlVao&& other) noexcept;
	GlVao& operator=(GlVao&& other) noexcept;

	// Core functionality
	void Bind();
	void Unbind();
	void Delete();

	template<typename T>
	void SetAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
		if (!m_IsBound)
			Bind();

		if (stride == 0) {
			stride = size * sizeof(T);
		}

		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void LinkAttrib(GlVbo& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, const void* pointer);

	// Enable/disable vertex attributes
	void EnableAttribute(GLuint index);
	void DisableAttribute(GLuint index);

	// Getters
	GLuint GetID() const;
	bool IsBound() const;
	bool IsValid() const;
};

#endif