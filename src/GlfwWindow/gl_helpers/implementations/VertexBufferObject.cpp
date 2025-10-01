#include "../VertexBufferObject.h"

// Constructor - generates VBO
VertexBufferObject::VertexBufferObject() : m_ID(0), m_IsBound(false) {

	glGenBuffers(1, &m_ID);

	if (m_ID == 0) {
		throw std::runtime_error("Failed to generate Vertex Buffer Object");
	}
}

// Destructor - automatically cleans up
VertexBufferObject::~VertexBufferObject() {
	if (m_ID != 0) {
		// If still bound, unbind first (safety measure)
		if (m_IsBound) {
			Unbind();
		}
		glDeleteBuffers(1, &m_ID);
	}
}


// Move constructor
VertexBufferObject::VertexBufferObject(VertexBufferObject&& other) noexcept
	: m_ID(other.m_ID), m_IsBound(other.m_IsBound) {
	other.m_ID = 0;
	other.m_IsBound = false;
}

// Move assignment
VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) noexcept {
	if (this == &other)
		return *this;

	if (m_ID != 0) {
		if (m_IsBound) {
			Unbind();
		}
		glDeleteBuffers(1, &m_ID);
	}

	m_ID = other.m_ID;
	m_IsBound = other.m_IsBound;

	other.m_ID = 0;
	other.m_IsBound = false;

	return *this;
}

// Bind the VBO
void VertexBufferObject::Bind(GLenum target ) {
	glBindBuffer(target, m_ID);
	m_IsBound = true;
}

// Unbind the VBO
void VertexBufferObject::Unbind(GLenum target ) {
	glBindBuffer(target, 0);
	m_IsBound = false;
}

// Set vertex data with explicit size
void VertexBufferObject::SetData(const void* data, GLsizeiptr size, GLenum usage ,GLenum target ) {
	Bind(target);
	glBufferData(target, size, data, usage);
}

// Get the VBO ID
GLuint VertexBufferObject::GetID() const { return m_ID; }

// Check if VBO is currently bound
bool VertexBufferObject::IsBound() const { return m_IsBound; }

// Check if VBO is valid (not deleted/moved)
bool VertexBufferObject::IsValid() const { return m_ID != 0; }

// Explicit delete (can be called manually, but RAII handles it automatically)
void VertexBufferObject::Delete() {
	if (m_ID != 0) {
		if (m_IsBound) Unbind();
		glDeleteBuffers(1, &m_ID);
		m_ID = 0;
	}
}


