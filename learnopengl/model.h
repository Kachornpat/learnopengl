#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.h"
#include "shader.h"

class Model {
private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
	void processNode(aiNode* node,const aiScene* scene);
	Mesh processMesh(aiMesh* mesh,const aiScene* scene);
	void loadModel(std::string path);
	std::vector<Texture> loadTextures(aiMaterial *material, aiTextureType type, std::string name);
	unsigned int textureFromFile(std::string path, std::string directory);

public:
	void Draw(Shader& shader);
	Model(std::string path);
};


#endif
