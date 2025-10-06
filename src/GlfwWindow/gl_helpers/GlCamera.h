#pragma once
#ifndef GL_CAMERA_H
#define GL_CAMERA_H

class GlCamera
{
private:


public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f , 0.0f , -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);;
};


#endif
