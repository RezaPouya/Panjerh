#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stb/stb_image.h>
#include "helpers/GlfwHelper.h"
#include "shaders/BasicShaders.cpp"
#include "gl_helpers/Shader.h"
#include "gl_helpers/VertexArrayObject.h"
#include "gl_helpers/VertexBufferObject.h"
#include "gl_helpers/ElementBufferObject.h"

// Vertices coordinates
std::vector<GLfloat> vertices05 =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
std::vector<GLuint> indices05 =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

int main() {

	GlfwHelper glfwHelper("Initial Win");

	try {
		VertexArrayObject vao;
		VertexBufferObject vbo;
		ElementBufferObject ebo;


		auto shaderProgram = Shader("shaders/shader_01.vert", "shaders/shader_01.frag");
		

		vbo.Bind();
		vbo.SetData(vertices05.data(), vertices05.size(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)0);
		vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(3 * sizeof(GLfloat))); // color 
		//vao.LinkAttrib(&vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(6 * sizeof(GLfloat))); // texture
		ebo.SetData(indices05, GL_STATIC_DRAW);
		vbo.Unbind();
		vao.Unbind(); // ebo stays bind to vao 
		
		// Gets ID of uniform called "scale"
		GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

		int textureWidht, textureHeight, textureNumberOfColorChannel;
		const unsigned char* textureData = stbi_load("resources/textures/pop_cat.png" , &textureWidht , &textureHeight , &textureNumberOfColorChannel , 0);
		GLuint texture;
		glGenTextures(1, &texture);
		
		// activate texture in texture unit ( Open GL has 64 texture unit ) 
		// texture unit : slot for texture that come toghter as bundle 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glfwHelper.RenderLoop([&shaderProgram, &vao, &ebo , &uniID]() {
			shaderProgram.Active();
			// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
			glUniform1f(uniID, 1.5f);
			vao.Bind();
			ebo.Draw();
		});

		// Cleanup
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		shaderProgram.Delete();
		glDeleteTextures(1 , &texture);
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}