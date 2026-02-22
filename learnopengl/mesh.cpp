#include "mesh.h"
#include <glm/glm.hpp>

Mesh::Mesh(std::vector<Vertex> vertices, 
	       std::vector<unsigned int> indices,
	       std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
}