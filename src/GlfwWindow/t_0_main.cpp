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

GLuint CreateVertexShader();
GLuint CreateFragmentShader();
GLuint CreateShaderProgram(GLuint& vertexShader, GLuint& fragmentShader);

int t_00_main()
{
	try {
		GlfwHelper glfwHelper("Open Gl Win");

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

		GLfloat verts[] = {
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // pos(x,y,z) + color(r,g,b) // bottom-left (red)
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // pos(x,y,z) + color(r,g,b) // bottom-right (green)
			0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // pos(x,y,z) + color(r,g,b) // top (blue)

			// second triangle
			0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // top left
		};

		//Memory: [-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, ...]
		//			↑					↑
		//			Position			Color


		// SET UP BUFFER AND ATTRIBUTES ONCE (not every frame)
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		if (posAttrib != -1 && colorAttrib != -1) {
			//  OpenGL interprets it based on your glVertexAttribPointer configuration:
			// This means we have to specify how OpenGL should interpret the vertex data before rendering. 
			// 6 * sizeof(float) : The fifth argument is known as the stride and tells us the space between consecutive vertex attributes
			// stride : just show how much these data belong to one vertext !!! 
			// (void*)0 : This is the offset of where the position data begins in the buffer.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);


			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(9 * sizeof(float)));

			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

		}
		else {
			std::cout << "Warning: Could not find attributes" << std::endl;
		}

		// Unbind VAO to prevent accidental modifications
		glBindVertexArray(0);

#pragma region ebo example

		// راس‌های مستطیل با EBO
		float rectangleVertices[] = {
			0.1f,  0.6f, 0.0f,   1.0f, 0.0f, 0.0f,  // راس 0: قرمز
			0.3f,  0.6f, 0.0f,   0.0f, 1.0f, 0.0f,  // راس 1: سبز
			0.1f,  0.8f, 0.0f,   0.0f, 0.0f, 1.0f,  // راس 2: آبی
			0.3f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f   // راس 3: زرد
		};

		unsigned int indices[] = {
			0, 1, 2,   // مثلث اول
			1, 2, 3    // مثلث دوم
		};

		// VAO جداگانه برای مستطیل با EBO
		GLuint rectangleVAO;
		glGenVertexArrays(1, &rectangleVAO);
		glBindVertexArray(rectangleVAO);

		GLuint EBO;
		glGenBuffers(1, &EBO);

		// برای EBO نیاز به VBO جداگانه داریم
		GLuint rectangleVBO;
		glGenBuffers(1, &rectangleVBO);

		// ابتدا VBO را تنظیم کنید
		glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

		// سپس EBO را تنظیم کنید
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// تنظیمات attribute برای مستطیل (از locationهای 0 و 1 استفاده کنید)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);


#pragma endregion



		// Unbind VAO to prevent accidental modifications
		glBindVertexArray(0);

		glfwHelper.RenderLoop([&shaderProgram, &VAO , &rectangleVAO]() {

			glUseProgram(shaderProgram);

			glBindVertexArray(VAO); // This automatically sets up all the vertex attributes

			glDrawArrays(GL_TRIANGLES, 0, 12); // Only one draw call needed

			// رسم مستطیل با EBO
// رسم مستطیل با EBO
			glBindVertexArray(rectangleVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
		std::cout << "GlShader program linked successfully!" << std::endl;

		GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
		GLint colorAttrib = glGetAttribLocation(shaderProgram, "aColor");
		std::cout << "aPos location: " << posAttrib << ", aColor location: " << colorAttrib << std::endl;
	}

	return shaderProgram;
}