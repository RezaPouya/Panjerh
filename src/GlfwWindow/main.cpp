#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "utils/GlfwHelper.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLuint CreateVertexShader();
GLuint CreateFragmentShader();
GLuint CreateShaderProgram(GLuint& vertexShader, GLuint& fragmentShader);

int main()
{
	try {
		GlfwHelper glfwHelper;

		// Create shaders first
		GLuint vertexShader = CreateVertexShader();
		GLuint fragmentShader = CreateFragmentShader();
		GLuint shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

		GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
		GLint colorAttrib = glGetAttribLocation(shaderProgram, "aColor"); // CHANGED: aColor instead of fragColor
		std::cout << "Attribute location for 'aColor': " << colorAttrib << std::endl;

		glEnable(GL_DEPTH_TEST);

		// 1. Generate and bind VAO (THIS IS MANDATORY)
		// VAOs store the state of vertex attribute configuration.
		GLuint VAO; // Vertex Array Object (REQUIRED for core profile)
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);

		GLfloat verts[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // pos(x,y,z) + color(r,g,b) // bottom-left (red)
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // pos(x,y,z) + color(r,g,b) // bottom-right (green)
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // pos(x,y,z) + color(r,g,b) // top (blue)
		};

		//Memory: [-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, ...]
		//			↑					↑
		//			Position			Color

		// SET UP BUFFER AND ATTRIBUTES ONCE (not every frame)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		if (posAttrib != -1 && colorAttrib != -1) {
			//  OpenGL interprets it based on your glVertexAttribPointer configuration:
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

			glEnableVertexAttribArray(posAttrib);
			glEnableVertexAttribArray(colorAttrib);
		}
		else {
			std::cout << "Warning: Could not find attributes" << std::endl;
		}

		// Unbind VAO to prevent accidental modifications
		glBindVertexArray(0);

		glfwHelper.RenderLoop([&shaderProgram, &VAO]() {

			glUseProgram(shaderProgram);

			glBindVertexArray(VAO); // This automatically sets up all the vertex attributes

			glDrawArrays(GL_TRIANGLES, 0, 3); // Only one draw call needed

			glBindVertexArray(0);

			});

		// Cleanup
		glDeleteVertexArrays(1, &VAO); // ADDED: Delete VAO
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		glDeleteBuffers(1, &VBO);

	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

GLuint CreateVertexShader() {
	// FIXED: Added aColor attribute
	const char* vertexShaderSource = R"(
        #version 330 core
        in vec3 aPos;
        in vec3 aColor;
        out vec3 vertexColor;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION FAILED: " << infoLog << std::endl;
	}
	else {
		std::cout << "Vertex shader compiled successfully!" << std::endl;
	}

	return vertexShader;
}

GLuint CreateFragmentShader() {
	// FIXED: Added vertexColor input
	const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 fragColor;
        void main()
        {
            fragColor = vec4(vertexColor, 1.0); // Use the vertex color instead of fixed color
        }
    )";

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED: " << infoLog << std::endl;
	}
	else {
		std::cout << "Fragment shader compiled successfully!" << std::endl;
	}

	return fragmentShader;
}

GLuint CreateShaderProgram(GLuint& vertexShader, GLuint& fragmentShader) {
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINKING FAILED: " << infoLog << std::endl;
	}
	else {
		std::cout << "Shader program linked successfully!" << std::endl;

		// Debug: Check attribute locations
		GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
		GLint colorAttrib = glGetAttribLocation(shaderProgram, "aColor");
		std::cout << "aPos location: " << posAttrib << ", aColor location: " << colorAttrib << std::endl;
	}

	return shaderProgram;
}