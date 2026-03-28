#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"

struct Model {
	unsigned int numMeshes;
	Mesh* meshes;
	unsigned int numMaterials;
	Material* materials;
};

void loadModel(Model *model, std::string filename);
void drawModel(Model model, Shader shader);
void deleteModel(Model model);
#endif