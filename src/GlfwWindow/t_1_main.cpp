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
#include "shaders/BasicShaders.cpp"

int t_01_main() {
	GlfwHelper glfwHelper("Initial Win");

	try {
		/*GLfloat vertecies[] = {
			-0.5f , 0.5f * GLfloat(sqrt(3)) / 3 , 0.0f ,
			0.5f , -0.5f * GLfloat(sqrt(3)) / 3 , 0.0f ,
			0.0f ,  -0.5f * GLfloat(sqrt(3)) * 2 / 3 , 0.0f ,
		};*/

		std::vector<GLfloat> vertices = {
			-0.5f, -0.5f * GLfloat(sqrt(3)) / 3, 0.0f, // every component have 3 object 
			0.5f, -0.5f * GLfloat(sqrt(3)) / 3,  0.0f,
			0.0f, 0.5f * GLfloat(sqrt(3)) * 2 / 3, 0.0f,
		};

		// all open gl objects are accessed by references ( by value )

		auto shaderProgram = BasicShaders::CreateShaderProgram(BasicShaders::VertexShaderSource(), BasicShaders::FragmentShaderSource());

		// Binding in openGL :
		//		we make certain object the current object 
		//		and whenever we call a function that modify that type of object , its current object of that type ( binded object ) 
		//		

		GLuint vertextArrayObject; // VAO : keep structure and tell openGl how to intecept raw data from VBO  
		// NOTE : VAO should be created before VBO , the ordering is important 
		// from the momeny it created , it start to capture structure of data ( automatically ) 
		glGenVertexArrays(1, &vertextArrayObject);
		glBindVertexArray(vertextArrayObject);

		GLuint vertexBufferObject;  // VBO : keep raw data in buffer for sending to gpu ( its block of raw memory data ) 
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		GLsizeiptr verteciesSize = vertices.size() * sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, verteciesSize , vertices.data(), GL_STATIC_DRAW);

		// now we want to configure VAO ( specifiy how open gl should interpert VBO ) 

		GLsizei stride = 3 * sizeof(GLfloat); // the amount of data between eachvertecies ! 
		auto offset = (void*)0;  // the pointer to where our vertecies start in array  
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(0); // for being able to use it 


		// optional : to prevent from accidentally change the value of VBA and VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0); // for vbo 
		glBindVertexArray(0); // ordering is important 


		glfwHelper.RenderLoop([&shaderProgram , &vertextArrayObject]() {
			glUseProgram(shaderProgram);
			glBindVertexArray(vertextArrayObject);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		});


		// Cleanup
		glDeleteVertexArrays(1, &vertextArrayObject); 
		glDeleteProgram(shaderProgram);
		glDeleteBuffers(1, &vertexBufferObject);
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}