#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>

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

struct Mesh {
	unsigned int vao;
	unsigned int indiceSize;
};

#endif // MESH_H

