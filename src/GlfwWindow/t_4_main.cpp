#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "helpers/GlfwHelper.h"
#include "shaders/BasicShaders.cpp"
#include "gl_helpers/GlShader.h"
#include "gl_helpers/GlVao.h"
#include "gl_helpers/GlVbo.h"
#include "gl_helpers/GlEbo.h"

int t4_main() {

	// Vertices coordinates
	std::vector<GLfloat> vertices04 =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};

	// Indices for vertices order
	std::vector<GLuint> indices04 =
	{
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
	};

	GlfwHelper glfwHelper("Initial Win");

	try {
		GlVao vao;
		GlVbo vbo;
		GlEbo ebo;

		auto shaderProgram = GlShader("shaders/shader_01.vert", "shaders/shader_01.frag");

		vbo.Bind();
		vbo.SetData(vertices04.data(), vertices04.size(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)0);
		vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(3 * sizeof(GLfloat))); // color 
		//vao.LinkAttrib(&vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat) , (void*)(6 * sizeof(GLfloat))); // texture
		ebo.SetData(indices04, GL_STATIC_DRAW);
		vbo.Unbind();
		vao.Unbind(); // ebo stays bind to vao 
		
		// Gets ID of uniform called "scale"
		GLuint uniID = glGetUniformLocation(shaderProgram.GetId(), "scale");

		glfwHelper.RenderLoop([&shaderProgram, &vao, &ebo , &uniID]() {
			shaderProgram.Active();
			// Assigns a value to the uniform; NOTE: Must always be done after activating the GlShader Program
			glUniform1f(uniID, 1.5f);
			vao.Bind();
			ebo.Draw();
		});

		// Cleanup
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		shaderProgram.Delete();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}