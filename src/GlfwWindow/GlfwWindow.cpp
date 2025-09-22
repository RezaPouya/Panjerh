#pragma once
#include <iostream>
#include <stdexcept> // For std::runtime_error
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
//#include <GL/glew.h> // we can not use both glad and glew together 
#include <cstdlib>
#include <cstdio>
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

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Set the viewport each frame in case window was resized
		glViewport(0, 0, width, height);
		// ------------------------------------------------------------------------------- 

		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT); // Actually clear the buffer!

		// -------------------------------------------------------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
}


