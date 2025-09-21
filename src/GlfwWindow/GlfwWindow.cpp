#include <iostream>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();
	//glfwWindowHint(GLFW)
	std::cout << "static linking _ glfw_init" << std::endl;
	std::cin.get();
	glfwTerminate();
}


