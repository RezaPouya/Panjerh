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

int t_2_main() {
	GlfwHelper glfwHelper("Initial Win");

	try {
		std::vector<GLfloat> vertices = {
			-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Lower right corner
			0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
			-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
			0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
			0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f     // Inner down
		};

		std::vector<GLuint> indicies = {
			 0, 3, 5, // lower left 
			 3, 2, 4, // lower right 
			 5, 4, 1 // top 
		};

		auto shaderProgram = BasicShaders::CreateShaderProgram(BasicShaders::VertexShaderSource(), BasicShaders::FragmentShaderSource());

		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLint), indicies.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat) /*stride*/, (void*)0 /*offset*/);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // it should be unbind after vao , because ebo is stored in VAO 

		glfwHelper.RenderLoop([&shaderProgram, &VAO]() {
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)0);
		});


		// Cleanup
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}