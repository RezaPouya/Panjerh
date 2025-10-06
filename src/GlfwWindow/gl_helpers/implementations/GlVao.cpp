#include "../GlVao.h"
#include "../GlVbo.h"

GlVao::GlVao(bool shouldBind) : m_ID(0), m_IsBound(false) {
	glGenVertexArrays(1, &m_ID);
	if (m_ID == 0) {
		throw std::runtime_error("Failed to generate Vertex Array Object");
	}

	if (shouldBind)
		Bind();
}

GlVao::~GlVao() {
	if (m_ID == 0) // nothing is binded !!! 
		return;

	if (m_IsBound)
		Unbind();

	glDeleteVertexArrays(1, &m_ID);
}

GlVao::GlVao(GlVao&& other) noexcept
	: m_ID(other.m_ID), m_IsBound(other.m_IsBound) {
	other.m_ID = 0;
	other.m_IsBound = false;
}

GlVao& GlVao::operator=(GlVao&& other) noexcept {
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

void GlVao::LinkAttrib(GlVbo& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, const void* offset)
{
	if (!m_IsBound)
		Bind();

	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


void GlVao::Bind() {
	glBindVertexArray(m_ID);
	m_IsBound = true;
}

void GlVao::Unbind() {
	glBindVertexArray(0);
	m_IsBound = false;
}

void GlVao::Delete() {
	if (m_ID != 0) {
		if (m_IsBound) Unbind();
		glDeleteVertexArrays(1, &m_ID);
		m_ID = 0;
	}
}

void GlVao::EnableAttribute(GLuint index) {
	if (!m_IsBound) Bind();
	glEnableVertexAttribArray(index);
}

void GlVao::DisableAttribute(GLuint index) {
	if (!m_IsBound) Bind();
	glDisableVertexAttribArray(index);
}

GLuint GlVao::GetID() const {
	return m_ID;
}

bool GlVao::IsBound() const {
	return m_IsBound;
}

bool GlVao::IsValid() const {
	return m_ID != 0;
}
