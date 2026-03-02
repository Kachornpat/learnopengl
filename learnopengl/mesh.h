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
	std::string path;
};

class Mesh {
public:
	unsigned int VBO, VAO, EBO;
	std::vector<Vertex> vertices;
	Mesh(std::vector<Vertex> vertexs);
};
#endif