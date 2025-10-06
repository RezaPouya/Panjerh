#include "../GlTexture.h"
#include "../GlShader.h"
#include <stb/stb_image.h>
#include <iostream>
#include <memory>

// ===== GlTexture Implementation =====

GlTexture::GlTexture(GLenum texture_type, GLenum texture_unit, int width, int height, int channels)
    : m_id(0), m_texture_type(texture_type), m_texture_unit(texture_unit),
    m_width(width), m_height(height), m_channels(channels),
    m_is_generated(false), m_is_bound(false) {
}

GlTexture::~GlTexture() {
    Delete();
}

GlTexture::GlTexture(GlTexture&& other) noexcept
    : m_id(other.m_id), m_texture_type(other.m_texture_type),
    m_texture_unit(other.m_texture_unit), m_width(other.m_width),
    m_height(other.m_height), m_channels(other.m_channels),
    m_is_generated(other.m_is_generated), m_is_bound(other.m_is_bound) {

    other.m_id = 0;
    other.m_is_generated = false;
    other.m_is_bound = false;
}

GlTexture& GlTexture::operator=(GlTexture&& other) noexcept {
    if (this != &other) {
        Delete();

        m_id = other.m_id;
        m_texture_type = other.m_texture_type;
        m_texture_unit = other.m_texture_unit;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;
        m_is_generated = other.m_is_generated;
        m_is_bound = other.m_is_bound;

        other.m_id = 0;
        other.m_is_generated = false;
        other.m_is_bound = false;
    }
    return *this;
}

void GlTexture::LoadImageFromFile(const std::string& file_path, bool flip_vertical) {
    stbi_set_flip_vertically_on_load(flip_vertical);

    unsigned char* image_data = stbi_load(file_path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (!image_data) {
        throw std::runtime_error("Failed to load texture: " + file_path);
    }

    // Determine format based on channels
    GLenum format = GL_RGB;
    GLenum internal_format = GL_RGB;
    if (m_channels == 4) {
        format = GL_RGBA;
        internal_format = GL_RGBA;
    }
    else if (m_channels == 1) {
        format = GL_RED;
        internal_format = GL_RED;
    }

    GenerateTexture(internal_format, format, GL_UNSIGNED_BYTE, image_data, true);
    stbi_image_free(image_data);
}

void GlTexture::GenerateTexture(GLenum internal_format, GLenum format, GLenum type,
    const void* data, bool generate_mipmaps) {
    if (m_is_generated) return;

    glGenTextures(1, &m_id);
    Bind();

    glTexImage2D(m_texture_type, 0, internal_format, m_width, m_height,
        0, format, type, data);

    if (generate_mipmaps && m_texture_type == GL_TEXTURE_2D) {
        glGenerateMipmap(m_texture_type);
    }

    m_is_generated = true;
}

void GlTexture::ApplyParameters(GLenum min_filter, GLenum mag_filter,
    GLenum wrap_s, GLenum wrap_t, GLenum wrap_r,
    const float* border_color) {
    if (!m_is_bound) Bind();

    glTexParameteri(m_texture_type, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(m_texture_type, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_T, wrap_t);

    if (m_texture_type == GL_TEXTURE_3D) {
        glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_R, wrap_r);
    }

    if ((wrap_s == GL_CLAMP_TO_BORDER || wrap_t == GL_CLAMP_TO_BORDER) && border_color) {
        glTexParameterfv(m_texture_type, GL_TEXTURE_BORDER_COLOR, border_color);
    }
}

void GlTexture::Bind() {
    glActiveTexture(m_texture_unit);
    glBindTexture(m_texture_type, m_id);
    m_is_bound = true;
}

void GlTexture::Unbind() {
    glBindTexture(m_texture_type, 0);
    m_is_bound = false;
}

void GlTexture::Delete() {
    if (m_id != 0) {
        if (m_is_bound) Unbind();
        glDeleteTextures(1, &m_id);
        m_id = 0;
        m_is_generated = false;
    }
}

void GlTexture::UpdateData(const void* data, GLenum format, GLenum type) {
    if (!m_is_generated) return;

    Bind();
    glTexSubImage2D(m_texture_type, 0, 0, 0, m_width, m_height, format, type, data);
}

void GlTexture::SetUniform(GlShader& shader, const std::string& uniform_name) const {
    shader.Active();
    GLint texture_unit_index = m_texture_unit - GL_TEXTURE0;
    glUniform1i(glGetUniformLocation(shader.GetId(), uniform_name.c_str()), texture_unit_index);
}

// ===== Builder Implementation =====

GlTexture::Builder GlTexture::Builder::FromFile(const std::string& file_path) {
    Builder builder;
    builder.m_file_path = file_path;
    return builder;
}

GlTexture::Builder GlTexture::Builder::Empty(int width, int height, int channels) {
    Builder builder;
    builder.m_width = width;
    builder.m_height = height;
    builder.m_channels = channels;
    return builder;
}

GlTexture::Builder& GlTexture::Builder::SetTextureType(GLenum type) {
    m_texture_type = type;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetTextureUnit(GLenum unit) {
    m_texture_unit = unit;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetFiltering(GLenum min_filter, GLenum mag_filter) {
    m_min_filter = min_filter;
    m_mag_filter = mag_filter;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetWrapS(GLenum wrap_s) {
    m_wrap_s = wrap_s;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetWrapT(GLenum wrap_t) {
    m_wrap_t = wrap_t;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetWrapR(GLenum wrap_r) {
    m_wrap_r = wrap_r;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetBorderColor(float r, float g, float b, float a) {
    m_border_color[0] = r;
    m_border_color[1] = g;
    m_border_color[2] = b;
    m_border_color[3] = a;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetFlipVertical(bool flip) {
    m_flip_vertical = flip;
    return *this;
}

GlTexture::Builder& GlTexture::Builder::SetGenerateMipmaps(bool generate) {
    m_generate_mipmaps = generate;
    return *this;
}

//std::unique_ptr<GlTexture> GlTexture::Builder::Build() {
//    auto texture = std::make_unique<GlTexture>(m_texture_type, m_texture_unit,
//        m_width, m_height, m_channels);
//
//    if (!m_file_path.empty()) {
//        // Load from file
//        texture->LoadImageFromFile(m_file_path, m_flip_vertical);
//    }
//    else {
//        // Create empty texture
//        GLenum format = GL_RGBA;
//        GLenum internal_format = GL_RGBA;
//        if (m_channels == 3) {
//            format = GL_RGB;
//            internal_format = GL_RGB;
//        }
//        else if (m_channels == 1) {
//            format = GL_RED;
//            internal_format = GL_RED;
//        }
//
//        texture->GenerateTexture(internal_format, format, GL_UNSIGNED_BYTE,
//            nullptr, m_generate_mipmaps);
//    }
//
//    // Apply all configured parameters
//    texture->ApplyParameters(m_min_filter, m_mag_filter, m_wrap_s, m_wrap_t,
//        m_wrap_r, m_border_color);
//
//    return texture;
//}


GlTexture GlTexture::Builder::Build() {
    auto texture = GlTexture(m_texture_type, m_texture_unit,
        m_width, m_height, m_channels);

    if (!m_file_path.empty()) {
        // Load from file
        texture.LoadImageFromFile(m_file_path, m_flip_vertical);
    }
    else {
        // Create empty texture
        GLenum format = GL_RGBA;
        GLenum internal_format = GL_RGBA;
        if (m_channels == 3) {
            format = GL_RGB;
            internal_format = GL_RGB;
        }
        else if (m_channels == 1) {
            format = GL_RED;
            internal_format = GL_RED;
        }

        texture.GenerateTexture(internal_format, format, GL_UNSIGNED_BYTE,
            nullptr, m_generate_mipmaps);
    }

    // Apply all configured parameters
    texture.ApplyParameters(m_min_filter, m_mag_filter, m_wrap_s, m_wrap_t,
        m_wrap_r, m_border_color);

    return texture;
}