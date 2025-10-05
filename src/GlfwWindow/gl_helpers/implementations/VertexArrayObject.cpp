#include "../VertexArrayObject.h"
#include "../VertexBufferObject.h"

VertexArrayObject::VertexArrayObject(bool shouldBind) : m_ID(0), m_IsBound(false) {
	glGenVertexArrays(1, &m_ID);
	if (m_ID == 0) {
		throw std::runtime_error("Failed to generate Vertex Array Object");
	}

	if (shouldBind)
		Bind();
}

VertexArrayObject::~VertexArrayObject() {
	if (m_ID == 0) // nothing is binded !!! 
		return;

	if (m_IsBound)
		Unbind();

	glDeleteVertexArrays(1, &m_ID);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
	: m_ID(other.m_ID), m_IsBound(other.m_IsBound) {
	other.m_ID = 0;
	other.m_IsBound = false;
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) noexcept {
	if (this != &other) {
		if (m_ID != 0) {
			if (m_IsBound) Unbind();
			glDeleteVertexArrays(1, &m_ID);
		}

		m_ID = other.m_ID;
		m_IsBound = other.m_IsBound;

		other.m_ID = 0;
		other.m_IsBound = false;
	}
	return *this;
}

void VertexArrayObject::LinkAttrib(VertexBufferObject& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, const void* offset)
{
	if (!m_IsBound)
		Bind();

	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


void VertexArrayObject::Bind() {
	glBindVertexArray(m_ID);
	m_IsBound = true;
}

void VertexArrayObject::Unbind() {
	glBindVertexArray(0);
	m_IsBound = false;
}

void VertexArrayObject::Delete() {
	if (m_ID != 0) {
		if (m_IsBound) Unbind();
		glDeleteVertexArrays(1, &m_ID);
		m_ID = 0;
	}
}

void VertexArrayObject::EnableAttribute(GLuint index) {
	if (!m_IsBound) Bind();
	glEnableVertexAttribArray(index);
}

void VertexArrayObject::DisableAttribute(GLuint index) {
	if (!m_IsBound) Bind();
	glDisableVertexAttribArray(index);
}

GLuint VertexArrayObject::GetID() const {
	return m_ID;
}

bool VertexArrayObject::IsBound() const {
	return m_IsBound;
}

bool VertexArrayObject::IsValid() const {
	return m_ID != 0;
}
