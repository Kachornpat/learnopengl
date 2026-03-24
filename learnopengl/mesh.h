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
};

struct Material {
	int id;
	Texture* diffuseTextures;
};

struct Mesh {
	unsigned int vao;
	unsigned int numIndices;
	unsigned int numTextures;
	Material* material;
};

void genMesh(const aiScene* scene, aiMesh* mesh_ai, Mesh *mesh, Material* materials, unsigned int numMaterial);
void drawMesh(Shader &shader, Mesh *mesh);

#endif // MESH_H

