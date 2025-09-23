#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLuint CreateVertexShader();
GLuint CreateFragmentShader();
GLuint CreateShaderProgram(GLuint& vertexShader, GLuint& fragmentShader);


int main()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1024, 760, "Hello world", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetKeyCallback(window, key_callback);
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Define Your Vertex Data
	GLfloat verts[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	// Create and bind VBO

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLuint vertexShader = CreateVertexShader();
	GLuint fragmentShader = CreateFragmentShader();
	GLuint shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

	// Get attribute location
	GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
	std::cout << "Attribute location for 'aPos': " << posAttrib << std::endl;

	// Enable depth test (just in case)
	glEnable(GL_DEPTH_TEST);


	GLuint VBO;
	glGenBuffers(1, &VBO);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		// -----------------------------------
		// render here 

		glUseProgram(shaderProgram);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		if (posAttrib != -1) {
			glEnableVertexAttribArray(posAttrib);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDisableVertexAttribArray(posAttrib);
		}
		else {
			std::cout << "Warning: Could not find attribute 'aPos'" << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}


GLuint CreateVertexShader() {

	GLint success;
	char infoLog[512];

	// Compile vertex shader

	const char* vertexShaderSource = R"(
        #version 330
        in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader compilation
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
	GLint success;
	char infoLog[512];

	const char* fragmentShaderSource = R"(
        #version 330
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(0.1, 0.1, 0.2, 1.0);
        }
    )";

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check fragment shader compilation
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

	// Check shader program linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINKING FAILED: " << infoLog << std::endl;
	}
	else {
		std::cout << "Shader program linked successfully!" << std::endl;
	}

	return shaderProgram;
}