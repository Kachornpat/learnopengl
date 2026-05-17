#ifndef MESH_H
#define MESH_H
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>

#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	int bones[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};


#endif // MESH_H

