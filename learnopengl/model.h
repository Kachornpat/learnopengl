#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

struct Model {
	unsigned int numMeshes;
	Mesh* meshes;
	unsigned int numMaterials;
	Material* materials;
};

int loadModel(Model *model, std::string filename);
void processNode(aiNode* node, const aiScene* scene, Mesh* meshes, Material* materials, std::string directory);
void drawModel(Model model, Shader shader);
void deleteModel(Model model);
void loadAnimation(Model* model, std::string filename);
#endif