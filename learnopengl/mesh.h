#ifndef MESH_H
#define MESH_H
#include <string>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture {
	unsigned int ID;
	std::string type;
};

class Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
public:
	Mesh(std::vector<Vertex> vertexs, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);
};
#endif