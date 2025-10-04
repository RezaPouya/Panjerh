#version 330 core
out vec4 FragColor; // output color in RGBA

in vec3 vertexColor;
in vec2 textureCoordiantes;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0 , textureCoordiantes);
}
