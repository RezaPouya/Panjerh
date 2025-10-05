#pragma once
#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <string>
#include <glad/gl.h>
#include <memory>
#include <vector>
#include <stdexcept>

// Forward declaration
class Shader;
 
class GlTexture {
   
private:
    GLuint m_id;
    GLenum m_texture_type;
    GLenum m_texture_unit;
    int m_width;
    int m_height;
    int m_channels;
    bool m_is_generated;
    bool m_is_bound;

    // Private constructor - only Builder can create instances
    GlTexture(GLenum texture_type, GLenum texture_unit, int width, int height, int channels);

public:
    // Builder class
    class Builder {
    private:
        GLenum m_texture_type = GL_TEXTURE_2D;
        GLenum m_texture_unit = GL_TEXTURE0;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        std::string m_file_path;
        GLenum m_min_filter = GL_LINEAR;
        GLenum m_mag_filter = GL_LINEAR;
        GLenum m_wrap_s = GL_REPEAT;
        GLenum m_wrap_t = GL_REPEAT;
        GLenum m_wrap_r = GL_REPEAT;
        float m_border_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        bool m_flip_vertical = true;
        bool m_generate_mipmaps = true;

    public:
        Builder() = default;
        friend class GlTexture;
        // Build from file
        static Builder FromFile(const std::string& file_path);

        // Build empty texture
        static Builder Empty(int width, int height, int channels = 4);

        // Configuration methods (return Builder& for chaining)
        Builder& SetTextureType(GLenum type);
        Builder& SetTextureUnit(GLenum unit);
        Builder& SetFiltering(GLenum min_filter, GLenum mag_filter);
        Builder& SetWrapS(GLenum wrap_s);
        Builder& SetWrapT(GLenum wrap_t);
        Builder& SetWrapR(GLenum wrap_r);
        Builder& SetBorderColor(float r, float g, float b, float a);
        Builder& SetFlipVertical(bool flip);
        Builder& SetGenerateMipmaps(bool generate);

        // Build the final texture
        //std::unique_ptr<GlTexture> Build();
        GlTexture Build();
    };

    ~GlTexture();

    // Delete copy operations
    GlTexture(const GlTexture&) = delete;
    GlTexture& operator=(const GlTexture&) = delete;

    // Move operations
    GlTexture(GlTexture&& other) noexcept;
    GlTexture& operator=(GlTexture&& other) noexcept;

    // Core functionality
    void Bind();
    void Unbind();
    void Delete();

    // Update texture data
    void UpdateData(const void* data, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

    // Set texture uniform in shader
    void SetUniform(Shader& shader, const std::string& uniform_name) const;

    // Getters
    GLuint GetID() const { return m_id; }
    GLenum GetTextureType() const { return m_texture_type; }
    GLenum GetTextureUnit() const { return m_texture_unit; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetChannels() const { return m_channels; }
    bool IsGenerated() const { return m_is_generated; }
    bool IsBound() const { return m_is_bound; }

private:
    // Internal methods
    void LoadImageFromFile(const std::string& file_path, bool flip_vertical);
    void GenerateTexture(GLenum internal_format, GLenum format, GLenum type,
        const void* data, bool generate_mipmaps);
    void ApplyParameters(GLenum min_filter, GLenum mag_filter,
        GLenum wrap_s, GLenum wrap_t, GLenum wrap_r,
        const float* border_color);
};

#endif

// usage example
//
// 
//// Simple texture from file
//auto texture1 = GlTexture::Builder::FromFile("texture.png")
//.Build();

//// Advanced configuration
//auto texture2 = GlTexture::Builder::FromFile("texture.jpg")
//.SetTextureUnit(GL_TEXTURE1)
//.SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
//.SetWrapS(GL_MIRRORED_REPEAT)
//.SetWrapT(GL_CLAMP_TO_EDGE)
//.SetFlipVertical(false)
//.Build();

//// Empty texture for framebuffers
//auto texture3 = GlTexture::Builder::Empty(1024, 1024, 4)
//.SetTextureType(GL_TEXTURE_2D)
//.SetFiltering(GL_NEAREST, GL_NEAREST)
//.Build();

//// 3D texture configuration
//auto texture4 = GlTexture::Builder::Empty(256, 256, 256, 4)
//.SetTextureType(GL_TEXTURE_3D)
//.SetWrapR(GL_CLAMP_TO_BORDER)
//.SetBorderColor(1.0f, 0.0f, 0.0f, 1.0f)
//.Build();