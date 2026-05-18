#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "animation.h"
#include "player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


double deltaTime = 0.0f;
double prevTime = 0.0f;

Camera camera;

int lastX = camera.screenX / 2;
int lastY = camera.screenY / 2;
float pitch = 0.0f, yaw = -90.0f;

float fov = 45.0f;

bool firstMouse = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(camera.screenX, camera.screenY, "learnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
	glViewport(0, 0, camera.screenX, camera.screenY);

	Shader shader("shader.vs", "shader.fs");

	std::string filename = "/home/kachornpat/Downloads/Lite Longbow Pack/Erika Archer.fbx";

	Model erika = Model(filename);

	Animation animation = Animation("/home/kachornpat/Downloads/Lite Longbow Pack/standing disarm bow.fbx", &erika);
	Player player = Player(&animation);

	float light[] = {
		-0.5, -0.5f,  0.5f,
		 0.5, -0.5f,  0.5f,
		-0.5,  0.5f,  0.5f,
		 0.5,  0.5f,  0.5f,
		-0.5, -0.5f, -0.5f,
		 0.5, -0.5f, -0.5f,
		-0.5,  0.5f, -0.5f,
		 0.5,  0.5f, -0.5f
	};

	unsigned int lightIndices[] = {
		0, 1, 2,
		2, 1, 3,
		4, 5, 6,
		6, 5, 7,
		1, 3, 5,
		3, 5, 7,
		4, 2, 0,
		2, 4, 6,
		2, 3, 6,
		3, 6, 7,
		0, 1, 3,
		1, 3, 5
	};

	unsigned int lightVAO, lightVBO, lightEBO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light), light, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &lightEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

	Shader lightShader("lightShader.vs", "lightShader.fs");

	glEnable(GL_DEPTH_TEST);


	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		player.updateTime(deltaTime);

		glm::vec3 lightPos(2.0f * cos(glfwGetTime()), 2.0f, 2.0f * sin(glfwGetTime()));

		shader.use();
		shader.setVec("viewPos", camera.Position);
		camera.updateView(&shader);
		camera.updateProjection(&shader);
		shader.setVec("pointLight.position", lightPos);
		shader.setVec("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));


		shader.setFloat("material.shininess", 32.0f);
		shader.setVec("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight.constant", 1.0f);
		shader.setFloat("pointLight.linear", 0.09f);
		shader.setFloat("pointLight.quadratic", 0.032f);

		std::vector<glm::mat4> transforms = player.getTranformations();
		for(unsigned int i = 0; i < transforms.size(); i++)
			shader.setMat("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glm::mat3 normalMat(glm::transpose(glm::inverse(model)));
		shader.setMat("normalMat", normalMat);
		shader.updateModel(model);
		erika.draw(shader);

		glBindVertexArray(lightVAO);
		lightShader.use();
		camera.updateView(&lightShader);
		camera.updateProjection(&lightShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		lightShader.updateModel(model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.screenX = width;
	camera.screenY = height;
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.Position += glm::normalize(camera.Up) * cameraSpeed;
		else
			camera.Position += glm::normalize(camera.Front) * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.Position -= glm::normalize(camera.Up) * cameraSpeed;
		else
			camera.Position -= glm::normalize(camera.Front) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = (int)xpos;
		lastY = (int)ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = (int)xpos;
	lastY = (int)ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.Front = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.fov -= (float)yoffset;
	if (camera.fov > 45.0f)
		camera.fov = 45.0f;
	if (camera.fov < 1.0f)
		camera.fov = 1.0f;
}





