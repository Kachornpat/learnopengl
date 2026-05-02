#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

class Model {

public:
	Model(std::string path) {
		loadModel(path);
	}
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path);
	Texture TextureFromEmbeded(aiTexture* texture);
};

#endif