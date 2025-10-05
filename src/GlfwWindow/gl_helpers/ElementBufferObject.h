#ifndef ELEMENT_BUFFER_OBJECT_H
#define ELEMENT_BUFFER_OBJECT_H

#pragma once
#include <glad/gl.h>
#include <vector>
#include <stdexcept>

class ElementBufferObject {
private:
    GLuint m_ID;
    bool m_IsBound;
    GLsizei m_IndexCount;
    // Core functionality
    void Bind();
    void Unbind();

public:
    // Constructor & Destructor
    ElementBufferObject();
    ~ElementBufferObject();

    // Delete copy operations
    ElementBufferObject(const ElementBufferObject&) = delete;
    ElementBufferObject& operator=(const ElementBufferObject&) = delete;

    // Move operations
    ElementBufferObject(ElementBufferObject&& other) noexcept;
    ElementBufferObject& operator=(ElementBufferObject&& other) noexcept;


    void Delete();

    // Template implementation
    template<typename T>
    void SetData(const std::vector<T>& indices, GLenum usage) {
        Bind();

        m_IndexCount = static_cast<GLsizei>(indices.size());
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(T),
            indices.data(),
            usage);
    }

    template<typename T>
    void UpdateData(const std::vector<T>& indices, GLintptr offset) {

        if (!m_IsBound) 
            Bind();

        m_IndexCount = static_cast<GLsizei>(indices.size());

        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
            offset,
            indices.size() * sizeof(T),
            indices.data());
    }

    // Drawing
    void Draw(GLenum mode = GL_TRIANGLES) const;

    // Getters
    GLuint GetID() const;
    bool IsBound() const;
    bool IsValid() const;
    GLsizei GetIndexCount() const;
};

#endif