#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <iostream>


#include "model.h"
#include "shader.h"

Model::Model(std::string path)
{
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of("/"));


}

void Model::processNode(aiNode* node, aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}

	for (unsigned int j = 0; j < node->mNumChildren; j++)
	{
		this->processNode(node->mChildren[j], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertice;
		vertice.position.x = mesh->mVertices[i].x;
		vertice.position.y = mesh->mVertices[i].y;
		vertice.position.z = mesh->mVertices[i].z;

		vertice.normal.x = mesh->mNormals[i].x;
		vertice.normal.y = mesh->mNormals[i].y;
		vertice.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertice.texCoord.x = mesh->mTextureCoords[0][i].x;
			vertice.texCoord.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertice);
	}

	return Mesh(vertices, indices, textures);
}