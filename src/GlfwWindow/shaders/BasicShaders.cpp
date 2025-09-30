#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>

struct BasicShaders {
public:
	static const GLchar* const* VertexShaderSource() {
		static const GLchar* source[] = {
			R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
        })",
		NULL
		};
		return source;
	}

	static const GLchar* const* FragmentShaderSource() {
		static const GLchar* source[] = { R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
        })", NULL
		};

		return source;
	}

	const char* AdvancedFragmentShader() {
		return R"(
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
	}

	static GLuint CreateShaderProgram(const GLchar *const* vertexShaderSource, const GLchar* const* fragmentShaderSource) {
		// all open gl objects are accessed by references ( by value )
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		GLint success;

		char infoLog[1024];

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "SHADER PROGRAM LINKING FAILED: " << infoLog << std::endl;
		}
		else {
			std::cout << "Shader program linked successfully!" << std::endl;

			GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
			GLint colorAttrib = glGetAttribLocation(shaderProgram, "aColor");
			std::cout << "aPos location: " << posAttrib << ", aColor location: " << colorAttrib << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return shaderProgram;
	}
};