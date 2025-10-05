#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "helpers/GlfwHelper.h"
#include "shaders/BasicShaders.cpp"
#include "gl_helpers/Shader.h"
#include "gl_helpers/VertexArrayObject.h"
#include "gl_helpers/VertexBufferObject.h"
#include "gl_helpers/ElementBufferObject.h"
#include "gl_helpers/GlTexture.h"


int main() {

	// Vertices coordinates
	std::vector<GLfloat> vertices =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	std::vector<GLuint> indices =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	const unsigned int width = 800;
	const unsigned int height = 800;

	GlfwHelper glfwHelper("Leaston 07 - Goding 3D ");

	try {
		VertexArrayObject vao;
		VertexBufferObject vbo;
		ElementBufferObject ebo;

		auto shaderProgram = Shader("shaders/shader_03.vert", "shaders/shader_03.frag");
		
		vbo.Bind();
		vbo.SetData(vertices.data(), vertices.size(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		ebo.SetData(indices, GL_STATIC_DRAW);
		vbo.Unbind();
		vao.Unbind(); // ebo stays bind to vao 

		GLuint uniID = glGetUniformLocation(shaderProgram.GetId(), "scale");
		shaderProgram.Active();

		GlTexture brickTexture = GlTexture::Builder::FromFile("resources/textures/brick.png")
			.SetTextureUnit(GL_TEXTURE0)
			.SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST)
			.SetWrapS(GL_MIRRORED_REPEAT)
			.SetWrapT(GL_CLAMP_TO_EDGE)
			.SetFlipVertical(true)
			.Build();

		brickTexture.SetUniform(shaderProgram, "texture0");
		glUniform1f(uniID, 1.0f);

		// Variables that help the rotation of the pyramid
		//float rotation = 0.0f;
		//double prevTime = glfwGetTime();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Variables that help the rotation of the pyramid
		float rotation = 0.0f;
		

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		// Outputs the matrices into the Vertex Shader
		GLuint modelUniformLocation = glGetUniformLocation(shaderProgram.GetId(), "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLuint viewUniformLocation = glGetUniformLocation(shaderProgram.GetId(), "view");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projUniformLocation = glGetUniformLocation(shaderProgram.GetId(), "proj");
		glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(proj));

		

		// Enables the Depth Buffer
		glEnable(GL_DEPTH_TEST);

		glfwHelper.RenderLoop([&vao, &ebo, &uniID ,  &brickTexture, &shaderProgram]() {
			double prevTime = glfwGetTime();

			//glUniform1f(uniID, 1.5f);
			brickTexture.Bind();
			vao.Bind();
			//ebo.Draw(GL_TRIANGLES);
			ebo.Draw(GL_TRIANGLES);


		});

		// Cleanup
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		shaderProgram.Delete();
		brickTexture.Delete();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}