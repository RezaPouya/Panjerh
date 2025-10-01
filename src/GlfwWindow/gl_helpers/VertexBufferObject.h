#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include <glad/gl.h>
#include <vector>
#include <stdexcept>

class VertexBufferObject {
private:
	GLuint m_ID;
	bool m_IsBound;

public:
	VertexBufferObject();
	~VertexBufferObject();
	
	// Delete copy constructor/assignment (OpenGL objects can't be copied)
	VertexBufferObject(const VertexBufferObject&) = delete;
	VertexBufferObject& operator=(const VertexBufferObject&) = delete;

	// Move constructor
	VertexBufferObject(VertexBufferObject&& other) noexcept;
	VertexBufferObject& operator=(VertexBufferObject&& other) noexcept;

	void Bind(GLenum target = GL_ARRAY_BUFFER);
	void Unbind(GLenum target = GL_ARRAY_BUFFER);
	
	// Set vertex data with explicit size
	void SetData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW,
		GLenum target = GL_ARRAY_BUFFER);

	template<typename T>
	void SetData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW,
		GLenum target = GL_ARRAY_BUFFER) {
		Bind(target);
		glBufferData(target, data.size() * sizeof(T), data.data(), usage);
	}

	// Set vertex data from raw pointer
	template<typename T>
	void SetData(const T* data, size_t count, GLenum usage = GL_STATIC_DRAW,
		GLenum target = GL_ARRAY_BUFFER) {
		Bind(target);
		glBufferData(target, count * sizeof(T), data, usage);
	}

	// Update portion of buffer data
	template<typename T>
	void UpdateData(const std::vector<T>& data, GLintptr offset = 0,
		GLenum target = GL_ARRAY_BUFFER) {
		if (!m_IsBound) Bind(target);
		glBufferSubData(target, offset, data.size() * sizeof(T), data.data());
	}

	// Update portion of buffer data from raw pointer
	template<typename T>
	void UpdateData(const T* data, size_t count, GLintptr offset = 0,
		GLenum target = GL_ARRAY_BUFFER) {
		if (!m_IsBound) Bind(target);
		glBufferSubData(target, offset, count * sizeof(T), data);
	}

	GLuint GetID() const;
	bool IsBound() const;
	bool IsValid() const;

	void Delete();
};

#endif