#ifndef MESH_H
#define MESH_H
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>

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
	unsigned int numIndices;
};


void genMesh(aiMesh* mesh_ai, Mesh* mesh);

#endif // MESH_H

