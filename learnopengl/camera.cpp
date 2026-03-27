#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "shader.h"

Camera::Camera()
{
	Position = glm::vec3(0.0f, 0.0f, 3.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	fov = 45.0f;
	screenX = 800;
	screenY = 600;
}

glm::mat4 Camera::getLookAt()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjection()
{
	return glm::perspective(glm::radians(fov), (float) screenX / screenY, 0.1f, 100.0f);
}

void Camera::updateView(Shader* shader)
{
	shader->updateView(getLookAt());
}

void Camera::updateProjection(Shader* shader)
{
	shader->updateProjection(getProjection());
}