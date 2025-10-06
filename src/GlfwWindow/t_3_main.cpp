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



int t3_main() {

	std::vector<GLfloat> vertices = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f     // Inner down
	};

	std::vector<GLuint> indices = {
		 0, 3, 5, // lower left 
		 3, 2, 4, // lower right 
		 5, 4, 1 // top 
	};

	GlfwHelper glfwHelper("Initial Win");

	try {
		GlVao vao;
		GlVbo vbo;
		GlEbo ebo;

		auto shader = GlShader("shaders/default.vert", "shaders/default.frag");

		vbo.Bind();
		vbo.SetData(vertices.data(), vertices.size(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vao.SetAttribute<GLfloat>(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat) , (void*)0);
		vao.EnableAttribute(0);
		ebo.SetData(indices, GL_STATIC_DRAW);

		vbo.Unbind();
		vao.Unbind(); // ebo stays bind to vao 

		glfwHelper.RenderLoop([&shader, &vao, &ebo]() {
			shader.Active();
			vao.Bind();
			ebo.Draw();
		});

		// Cleanup
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		shader.Delete();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}