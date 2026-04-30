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
	unsigned int id;
	unsigned int diffuseMap;
	unsigned int specularMap;
};

class Mesh {

private:
	unsigned int VAO, numIndices;
	Material* material;

public:
	Mesh(const aiScene* scene, aiMesh* mesh, std::vector<Material> &materials);
	void draw(Shader& shader);
};



#endif // MESH_H

