#include "../ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()
    : m_ID(0), m_IsBound(false), m_IndexCount(0) {
    glGenBuffers(1, &m_ID);
    if (m_ID == 0) {
        throw std::runtime_error("Failed to generate Element Buffer Object");
    }
}

ElementBufferObject::~ElementBufferObject() {
    if (m_ID != 0) {
        if (m_IsBound) {
            Unbind();
        }
        glDeleteBuffers(1, &m_ID);
    }
}

ElementBufferObject::ElementBufferObject(ElementBufferObject&& other) noexcept
    : m_ID(other.m_ID), m_IsBound(other.m_IsBound), m_IndexCount(other.m_IndexCount) {
    other.m_ID = 0;
    other.m_IsBound = false;
    other.m_IndexCount = 0;
}

ElementBufferObject& ElementBufferObject::operator=(ElementBufferObject&& other) noexcept {
    if (this != &other) {
        if (m_ID != 0) {
            if (m_IsBound) Unbind();
            glDeleteBuffers(1, &m_ID);
        }

        m_ID = other.m_ID;
        m_IsBound = other.m_IsBound;
        m_IndexCount = other.m_IndexCount;

        other.m_ID = 0;
        other.m_IsBound = false;
        other.m_IndexCount = 0;
    }
    return *this;
}

void ElementBufferObject::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    m_IsBound = true;
}

void ElementBufferObject::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_IsBound = false;
}

void ElementBufferObject::Delete() {
    if (m_ID != 0) {
        if (m_IsBound) Unbind();
        glDeleteBuffers(1, &m_ID);
        m_ID = 0;
        m_IndexCount = 0;
    }
}

void ElementBufferObject::Draw(GLenum mode) const {
    if (m_IndexCount > 0 && m_IsBound) {
        glDrawElements(mode, m_IndexCount, GL_UNSIGNED_INT, 0);
    }
}

GLuint ElementBufferObject::GetID() const {
    return m_ID;
}

bool ElementBufferObject::IsBound() const {
    return m_IsBound;
}

bool ElementBufferObject::IsValid() const {
    return m_ID != 0;
}

GLsizei ElementBufferObject::GetIndexCount() const {
    return m_IndexCount;
}

// Template implementations must be in header