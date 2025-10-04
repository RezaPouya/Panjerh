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
{ //     COORDINATES     /        COLORS      /   TextureCoord  // if you give coordinates higher than 1 , texture will be repeated ! 
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

		auto shaderProgram = Shader("shaders/shader_02.vert", "shaders/shader_02.frag");
		

		vbo.Bind();
		vbo.SetData(vertices05.data(), vertices05.size(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)0);
		vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(3 * sizeof(GLfloat))); // color 
		vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(6 * sizeof(GLfloat))); // texture
		ebo.SetData(indices05, GL_STATIC_DRAW);
		vbo.Unbind();
		vao.Unbind(); // ebo stays bind to vao 
		
		
		// |NOTE| : square texture with power 2 ( 1x1 , 2x2 , 4x4 , 64x64 , 256x256 , ... ) are better optimized than texture with random dimension. 

		int textureWidth, textureHeight, textureNumberOfColorChannel;
		stbi_set_flip_vertically_on_load(true);
		stbi_set_flip_vertically_on_load_thread(true);
		unsigned char* imageData = stbi_load("resources/textures/pop_cat.png" , &textureWidth, &textureHeight , &textureNumberOfColorChannel , 0);

		GLuint texture; // refrence valuable for texture 
		glGenTextures(1, &texture);
		
		// activate texture in texture unit ( Open GL has 64 texture unit ) 
		// texture unit : slot for texture that come toghter as bundle 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// texture render type settings : GL_NEAREST , GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// texture mirroring type settings : 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// float flatColor[] = {1.0f , 1.0f , 1.0f } 
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER );
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor );

		// now we can generate texture 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D); // min map smaller resolution of texture for when texture is fare away from camera ( for example ) 

		// now because we already imported image data , we can now delete it from memory 
		stbi_image_free(imageData);

		// now we unbind texture so we don't accidentally change it data 
		glBindTexture(GL_TEXTURE_2D, 0);

		// Gets ID of uniform called "scale"
		GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
		GLuint unitTexture0Id = glGetUniformLocation(shaderProgram.ID, "texture0");
		shaderProgram.Active();
		
		glUniform1i(unitTexture0Id, 0);


		glfwHelper.RenderLoop([&vao, &ebo, &uniID , &texture]() {
			glUniform1f(uniID, 1.5f);
			glBindTexture(GL_TEXTURE_2D, texture);
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