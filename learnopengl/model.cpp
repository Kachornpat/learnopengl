#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <iostream>

#include "stb_image.h"

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
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node,const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		Mesh mesh = processMesh(scene->mMeshes[node->mMeshes[i]], scene);
		meshes.push_back(mesh);
	}

	for (unsigned int j = 0; j < node->mNumChildren; j++)
	{
		processNode(node->mChildren[j], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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
		else
			vertice.texCoord = glm::vec2(0.0f, 0.0f);


		vertices.push_back(vertice);
	}

	for (unsigned int j = 0; j < mesh->mNumFaces; j++)
	{
		aiFace face = mesh->mFaces[j];
		for (unsigned int k = 0; k < face.mNumIndices; k++)
			indices.push_back(face.mIndices[k]);
	}

	if (mesh->mMaterialIndex>=0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.begin(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.begin(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial *material, aiTextureType type, std::string name)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		bool skip = false;

		aiString path;
		material->GetTexture(type, i, &path);

		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (!std::strcmp(loadedTextures[j].path.data(), path.C_Str()))
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}

		}

		if (!skip)
		{
			Texture texture;
			texture.id = textureFromFile(path.C_Str(), directory);
			texture.type = name;
			texture.path = path.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::textureFromFile(std::string path, std::string directory)
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
	std::cout << "Model::textureFromFile path " << (directory + "/" + path).c_str() << std::endl;
	unsigned char* data = stbi_load((directory + "/" + path).c_str(), &width, &height, &nrChannels, 0);

	GLenum format = GL_RGB;

	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	return textureMap;
}