#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "shader.h"
#include "model.h"

unsigned int indexMesh = 0;
unsigned int numMeshes = 0;


void processNode(aiNode* node, const aiScene* scene, Mesh* meshes, Material* materials, std::string directory)
{
	for (unsigned int i = 0;i < node->mNumMeshes; i++)
	{
		genMesh(scene, scene->mMeshes[node->mMeshes[i]], meshes + i, materials, directory);
		indexMesh++;
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, meshes + indexMesh,  materials, directory);
	}
}

void loadModel(Model* model, std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	numMeshes = scene->mNumMeshes;

	model->meshes = (Mesh*)malloc(scene->mNumMeshes * sizeof(Mesh));
	model->materials = (Material*)malloc(scene->mNumMaterials * sizeof(Material));
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		if (model->materials != NULL)
			model->materials[i].id = -1;
	}
	std::string directory = filename.substr(0, filename.find_last_of('/'));
	processNode(scene->mRootNode, scene, model->meshes, model->materials, directory);
}

void drawModel(Model model, Shader shader)
{
	for (unsigned int i = 0; i < numMeshes; i++)
		drawMesh(shader, model.meshes + i);
}

void deleteModel(Model model)
{
	free(model.meshes);
	free(model.materials);
}