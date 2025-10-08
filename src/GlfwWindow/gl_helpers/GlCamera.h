#pragma once
#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/round.hpp>
#include <glm/gtc/quaternion.hpp>       // for glm::angle
#include <glm/gtx/vector_angle.hpp>     // alternative for glm::angle
#include <GLFW/glfw3.h>

class GlShader;
class GLFWwindow;

class GlCamera
{
private:


public:

	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f , 0.0f , -1.0f); // direction
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	GlCamera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, GlShader& shader, const char* uniform);

	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};


#endif
