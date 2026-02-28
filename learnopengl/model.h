#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <assimp/Importer.hpp>
#include "mesh.h"
#include "shader.h"

class Model {
private:
	std::string directory;
	std::vector<Mesh> meshes;
	void processNode(aiNode* node, aiScene* scene);
	Mesh processMesh(aiMesh* mesh, aiScene* scene);
	void loadModel(std::string path);

public:
	void Draw(Shader& shader);
	Model(std::string path);
};


#endif
