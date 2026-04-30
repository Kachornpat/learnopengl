#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "stb_image.h"
#include "shader.h"

#include "mesh.h"
#include "model.h"

Model::Model(std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = filename.substr(0, filename.find_last_of('/'));

	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {

		unsigned int diffuseNum = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		unsigned int specularNum = scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);

		std::cout << "Load new Material " << i << std::endl;

		Material material;

		material.id = i;

		for (unsigned int j = 0; j < diffuseNum; j++)
		{
			aiString texturePath;
			scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &texturePath);
			std::string path = directory + "/" + texturePath.C_Str();
			std::cout << path << std::endl;
			material.diffuseMap = loadTexture(path.c_str(), false);
		}


		for (unsigned int j = 0; j < specularNum; j++)
		{
			aiString texturePath;
			scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, j, &texturePath);
			std::string path = directory + "/" + texturePath.C_Str();
			std::cout << path << std::endl;
			material.specularMap = loadTexture(path.c_str(), false);
		}

		materials.push_back(material);
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0;i < node->mNumMeshes; i++)
		meshes.push_back(Mesh(scene, scene->mMeshes[node->mMeshes[i]], materials));
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}


void Model::draw(Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader);
}

unsigned int Model::loadTexture(const char* path, bool flip)
{
	unsigned int textureMap;
	glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum fileFormat;
		if (nrChannels == 1)
			fileFormat = GL_RED;
		else if (nrChannels == 3)
			fileFormat = GL_RGB;
		else
			fileFormat = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, fileFormat, width, height, 0, fileFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Cannot load image: " << path << std::endl;
		return -1;
	}
	stbi_image_free(data);

	return textureMap;
}