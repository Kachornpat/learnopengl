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
private:
	unsigned int VBO, VAO, EBO;
	void setupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertexs, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
	void Draw(Shader &shader);
};
#endif