#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "stb_image.h"
#include "shader.h"
#include "model.h"

Model::Model(std::string filename)
{
	this->filename = filename;
	this->directory = filename.substr(0, filename.find_last_of('/'));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		aiMaterial* material = scene->mMaterials[meshes[i].materialIndex];
		processTexture(material, aiTextureType_DIFFUSE);
		processTexture(material, aiTextureType_SPECULAR);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		meshes.push_back(Mesh(scene->mMeshes[node->mMeshes[i]]));
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}


unsigned int Model::loadTexture(const char* path)
{
	unsigned int textureMap;
	glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	return textureMap;
}

void Model::processTexture(aiMaterial* material, aiTextureType textureType) {
	for (unsigned int j = 0; j < material->GetTextureCount(textureType); j++)
	{
		bool skip = false;
		aiString texturePath;
		material->GetTexture(textureType, j, &texturePath);
		std::string path = directory + "/" + texturePath.C_Str();
		for (unsigned int k = 0; k < textures.size(); k++)
		{
			if (!std::strcmp(textures[k].path.data(), path.c_str()))
			{
				skip = true;
			}
		}

		if (!skip)
		{
			Texture texture;
			unsigned int textureMap = loadTexture(path.c_str());
			texture.id = textureMap;
			texture.type = "texture_specular";
			texture.path = path;
			textures.push_back(texture);
		}
	}
}

void Model::draw(Shader &shader) {

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		std::string number;
		std::string texture_type = textures[i].type;
		if (texture_type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (texture_type == "texture_specular")
			number = std::to_string(specularNr++);
		shader.setInt(("material." + texture_type + number), i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw();
	}
}