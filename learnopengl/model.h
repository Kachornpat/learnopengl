#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

class Model {

public:
	Model(std::string filename);
	void draw(Shader& shader);

private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	void processNode(aiNode* node, const aiScene* scene);
	unsigned int loadTexture(const char* path, bool flip);

};

#endif