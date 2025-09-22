#pragma once
#include <iostream>
#include <stdexcept> // For std::runtime_error
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
//#include <GL/glew.h> // we can not use both glad and glew together 
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void DrawTriangle();

int main()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use Core Profile
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1024, 760, "Hello world", nullptr, nullptr);
	
	if (window == nullptr) {
		glfwTerminate(); // Clean up GLFW before throwing
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetErrorCallback(error_callback);
	
	glfwMakeContextCurrent(window);

	// 3. Initialize GLAD
	if (!gladLoadGL(glfwGetProcAddress)) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetKeyCallback(window, key_callback);

	// Print OpenGL version to verify it's working
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	///// SETUP CODE (Do this ONCE, before the loop) /////

	GLfloat verts[] = {
		 0.0f,  0.5f,  // Vertex 1: Top center
		-0.5f, -0.5f,  // Vertex 2: Bottom left
		 0.5f, -0.5f   // Vertex 3: Bottom right
	};

	GLuint VAO; // Vertex Array Object (REQUIRED for core profile)
	GLuint VBO; // Vertex Buffer Object

	// 1. Generate and bind VAO (THIS IS MANDATORY)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. Generate and bind VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Fixed: GL_ARRAY_BUFFER, not GL_ARRAY_BUFFER_BINDING

	// 3. Copy vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// 4. Set vertex attribute pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 5. Unbind VAO (optional but good practice)
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set viewport
		glViewport(0, 0, width, height);

		///// RENDER CODE (Do this every frame) /////

		// 6. Bind the VAO (this remembers all the VBO and attribute settings)
		glBindVertexArray(VAO);

		// 7. Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 8. Unbind VAO (optional)
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}


static void DrawTriangle() {


}
