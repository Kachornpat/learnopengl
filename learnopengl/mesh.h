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


struct Material {
	int id;
	unsigned int diffuseNum;
	unsigned int *diffuseTextures;
	unsigned int specularNum;
	unsigned int *specularTextures;
};

struct Mesh {
	unsigned int vao;
	unsigned int numIndices;
	Material* material;
};

struct Node {
	unsigned int numMeshes;
	Mesh* meshes;
	unsigned int numChildren;
	Node* children;
};

struct Bone {
	unsigned int id;
	Node* nodes;
};

void genMesh(const aiScene* scene, aiMesh* mesh_ai, Mesh *mesh, Material* materials, std::string directory);
void drawMesh(Shader &shader, Mesh *mesh);
void loadTexture(unsigned int *textureMap, const char* path, bool flip);

#endif // MESH_H

