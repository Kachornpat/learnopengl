#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Model {
private:
	std::string filename;
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

	unsigned int loadTexture(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	void processTexture(aiMaterial* material, aiTextureType textureType);
public:
	Model(std::string filename);
	void draw(Shader& shader);
};

#endif