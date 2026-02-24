#ifndef MESH_H
#define MESH_H
#include <string>
#include <glm/glm.hpp>
#include "shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
	unsigned int VBO, VAO, EBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	void setupMesh();
public:
	Mesh(std::vector<Vertex> vertexs, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
	void Draw(Shader &shader);
};
#endif