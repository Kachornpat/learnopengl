#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <map>

#include "shader.h"
#include "mesh.h"
#include "bone.h"

class Model {

public:
	Model(std::string path) {
		loadModel(path);
	}
	void draw(Shader& shader);
	std::vector<Bone> getBones();
	int getBoneID(std::string);


private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	std::vector<Bone> bones;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void resetBones(Vertex &vertice);
	void processBone(Vertex &vertice, int boneID, float weight);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path);
	Texture TextureFromEmbeded(aiTexture* texture);
};

#endif