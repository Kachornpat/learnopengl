#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include "shader.h"

class Camera
{
private:
	glm::mat4 getLookAt();
	glm::mat4 getProjection();

public: 
	float fov;
	float screenX;
	float screenY;
	Camera();
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	void updateView(Shader* shader);
	void updateProjection(Shader* shader);
};

#endif