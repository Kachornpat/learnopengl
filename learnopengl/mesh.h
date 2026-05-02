#ifndef MESH_H
#define MESH_H
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>

#include "shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	unsigned int *diffuseTextures;
	unsigned int specularNum;
class Mesh {
	unsigned int numIndices;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void draw(Shader& shader);
	unsigned int numChildren;
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};
	unsigned int id;
	Node* nodes;
};


#endif // MESH_H

