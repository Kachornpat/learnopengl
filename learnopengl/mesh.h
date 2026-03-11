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

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Mesh_s {
	unsigned int vao;
	unsigned int indiceSize;
};

Mesh_s processMesh(aiMesh* mesh);
void drawMesh(Mesh_s mesh);

class Mesh {
private:
	unsigned int VBO, EBO;
public:
	unsigned int VAO, indiceSize, materialIndex;
	Mesh(aiMesh* mesh);
	void draw() const;
};
#endif // MESH_H

