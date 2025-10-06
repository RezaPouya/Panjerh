#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 vertexColor; // Outputs the color for the Fragment GlShader
out vec2 textureCoordiantes ; // Output the texture coordinates 

// Controls the scale of the vertices
uniform float scale;

// Inputs the matrices needed for 3D viewing with perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
    textureCoordiantes = aTexture;
}