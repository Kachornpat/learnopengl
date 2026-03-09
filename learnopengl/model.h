#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"


class Model {
private:
	std::string filename;
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

	unsigned int loadTexture(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<Texture> processTexture(aiMaterial* material, aiTextureType textureType);
public:
	Model(std::string filename);
	void draw(Shader& shader);
};

#endif