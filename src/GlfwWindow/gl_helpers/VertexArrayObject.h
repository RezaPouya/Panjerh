#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#pragma once
#include <glad/gl.h>
#include <vector>
#include <stdexcept>

class VertexArrayObject {
private:
	GLuint m_ID;
	bool m_IsBound;

public:
	// Constructor & Destructor
	VertexArrayObject(bool shouldBind = true);
	~VertexArrayObject();

	// Delete copy operations
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	// Move operations
	VertexArrayObject(VertexArrayObject&& other) noexcept;
	VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;

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

	// Enable/disable vertex attributes
	void EnableAttribute(GLuint index);
	void DisableAttribute(GLuint index);

	// Getters
	GLuint GetID() const;
	bool IsBound() const;
	bool IsValid() const;
};

#endif