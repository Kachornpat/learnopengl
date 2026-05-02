#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "stb_image.h"
#include "shader.h"

#include "mesh.h"
#include "model.h"

void Model::draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

void Model::loadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = filename.substr(0, filename.find_last_of('/'));
    for (unsigned int i = 0; i < scene->mNumTextures; i++)
    {
        aiTexture *texture = scene->mTextures[i];
        textures_loaded.push_back(TextureFromEmbeded(texture));
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0;i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
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
        {
            vertice.texCoord = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertice);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].
            mNumIndices; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile((directory + "/" + str.C_Str()).c_str());
            std::cout << "load texture: " << (directory + "/" + str.C_Str()) << std::endl;
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char* path)
{
    unsigned int textureMap;
    glGenTextures(1, &textureMap);
    glBindTexture(GL_TEXTURE_2D, textureMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);

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
    }
    stbi_image_free(data);

    return textureMap;
}


Texture Model::TextureFromEmbeded(aiTexture* texture)
{
    Texture processedTexture;
    unsigned int textureMap = 0;
    glGenTextures(1, &textureMap);
    glBindTexture(GL_TEXTURE_2D, textureMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);

    unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &nrChannels, 0);

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
        std::cout << "Cannot load embeded texture" << std::endl;
    }
    stbi_image_free(data);

    processedTexture.id = textureMap;
    processedTexture.path = texture->mFilename.C_Str();
    processedTexture.type = "texture_diffuse";

    return processedTexture;
}

