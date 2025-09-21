#include <iostream>
#include <stdexcept> // For std::runtime_error
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

int main()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use Core Profile

	GLFWwindow* window = glfwCreateWindow(1024, 760, "Hello world", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate(); // Clean up GLFW before throwing
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);

	// 3. Initialize GLAD
	if (!gladLoadGL(glfwGetProcAddress)) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// ------------------------------------------------------------------------------- 



		// -------------------------------------------------------------------------------


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
}


