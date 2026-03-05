#ifndef MESH_H
#define MESH_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh {
private:
	unsigned int VBO, EBO;
public:
	unsigned int VAO, indiceSize;
	Mesh(aiMesh* mesh);
	void draw() const;
};
#endif // MESH_H

