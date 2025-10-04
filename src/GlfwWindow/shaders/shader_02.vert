#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 vertexColor; // Outputs the color for the Fragment Shader
out vec2 textureCoordiantes ; // Output the texture coordinates 

// Controls the scale of the vertices
uniform float scale;

void main()
{
    gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z * scale, 1.0f);
    vertexColor = aColor;
    textureCoordiantes = aTexture;
}