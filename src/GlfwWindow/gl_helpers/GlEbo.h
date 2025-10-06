#ifndef GL_Ebo_H
#define GL_Ebo_H

#pragma once
#include <glad/gl.h>
#include <vector>
#include <stdexcept>

/// <summary>
/// Wrapper for OpenGl Element Buffer Object  
/// </summary>
class GlEbo {
private:
    GLuint m_ID;
    bool m_IsBound;
    GLsizei m_IndexCount;


public:
    // Constructor & Destructor
    GlEbo();
    ~GlEbo();

    // Delete copy operations
    GlEbo(const GlEbo&) = delete;
    GlEbo& operator=(const GlEbo&) = delete;

    // Move operations
    GlEbo(GlEbo&& other) noexcept;
    GlEbo& operator=(GlEbo&& other) noexcept;

    // Core functionality
    void Bind();
    void Unbind();

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