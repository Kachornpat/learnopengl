#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(const char* path);
void processTexture(aiMaterial* material, aiTextureType textureType);

float deltaTime = 0.0f;
float prevTime = 0.0f;

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

	GLFWwindow* window = glfwCreateWindow((int)camera.screenX, (int)camera.screenY, "learnOpenGL", NULL, NULL);
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

	std::string filename = "C:/Users/kachornpat.g/Downloads/backpack/backpack.obj";
	std::string directory = filename.substr(0, filename.find_last_of('/'));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
		return -1;
	}

	aiMesh* mesh = scene->mMeshes[1];

	//std::vector<Vertex> vertices;
	Vertex* vertices = (Vertex*)malloc(mesh->mNumVertices * sizeof(Vertex));
	unsigned int numIndices = 3 * mesh->mNumFaces;
	unsigned int* indices = (unsigned int*)malloc(numIndices * sizeof(unsigned int));



	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].position.x = mesh->mVertices[i].x;
		vertices[i].position.y = mesh->mVertices[i].y;
		vertices[i].position.z = mesh->mVertices[i].z;

		vertices[i].normal.x = mesh->mNormals[i].x;
		vertices[i].normal.y = mesh->mNormals[i].y;
		vertices[i].normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertices[i].texCoord.x = mesh->mTextureCoords[0][i].x;
			vertices[i].texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertices[i].texCoord = glm::vec2(0.0f, 0.0f);
	}

	unsigned int indexIndice = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices[indexIndice] = mesh->mFaces[i].mIndices[j];
			indexIndice++;
		}
	}

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  mesh->mNumVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	free(vertices);
	free(indices);

	unsigned int diffuseMap = 0, specularMap = 0;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); j++)
		{
			bool skip = false;
			aiString texturePath;

			material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath);
			std::string path = directory + "/" + texturePath.C_Str();
			diffuseMap = loadTexture(path.c_str());

		}
		for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_SPECULAR); j++)
		{
			bool skip = false;
			aiString texturePath;

			material->GetTexture(aiTextureType_SPECULAR, j, &texturePath);
			std::string path = directory + "/" + texturePath.C_Str();
			specularMap = loadTexture(path.c_str());
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		
	}


	
	shader.setFloat("material.shininess", 32.0f);

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
		float currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
		processInput(window);
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		
		shader.use();
		shader.setVec("viewPos", camera.Position);
		camera.updateView(&shader);
		camera.updateProjection(&shader);
		glm::vec3 lightPos(2.0f * cos(glfwGetTime()), 2.0f, 2.0f * sin(glfwGetTime()));


		shader.setVec("pointLight.position", lightPos);
		shader.setVec("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("pointLight.constant", 1.0f);
		shader.setFloat("pointLight.linear", 0.09f);
		shader.setFloat("pointLight.quadratic", 0.032f);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat3 normalMat(glm::transpose(glm::inverse(model)));
		shader.setMat("normalMat", normalMat);
		shader.updateModel(model);

		shader.setInt("material.texture_diffuse1", 0);
		shader.setInt("material.texture_specular1", 1);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

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

unsigned int loadTexture(const char* path)
{
	unsigned int textureMap;
	glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum fileFormat;
		if (nrChannels == 1)
			fileFormat = GL_RED;
		else if (nrChannels == 3)
			fileFormat = GL_RGB;
		else
			fileFormat = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, fileFormat, width, height, 0, fileFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	return textureMap;
}
