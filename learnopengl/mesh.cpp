#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"

unsigned int textureId = 0;

void genMesh(const aiScene* scene, aiMesh *mesh_ai, Mesh* mesh, Material* materials, std::string directory)
{
    std::vector<Vertex> verticess;
    std::vector<unsigned int> indicess;

    for (unsigned int i = 0; i < mesh_ai->mNumVertices; i++)
    {
        Vertex vertice;

        vertice.position.x = mesh_ai->mVertices[i].x;
        vertice.position.y = mesh_ai->mVertices[i].y;
        vertice.position.z = mesh_ai->mVertices[i].z;


        vertice.normal.x = mesh_ai->mNormals[i].x;
        vertice.normal.y = mesh_ai->mNormals[i].y;
        vertice.normal.z = mesh_ai->mNormals[i].z;

        if (mesh_ai->mTextureCoords[0])
        {
            vertice.texCoord.x = mesh_ai->mTextureCoords[0][i].x;
            vertice.texCoord.y = mesh_ai->mTextureCoords[0][i].y;
        }
        else
        {
            vertice.texCoord = glm::vec2(0.0f, 0.0f);
        }

        verticess.push_back(vertice);
    }

    unsigned int indexIndice = 0;
    for (unsigned int i = 0; i < mesh_ai->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh_ai->mFaces[i].mNumIndices; j++)
        {
            indicess.push_back(mesh_ai->mFaces[i].mIndices[j]);
        }
    }

    if (mesh_ai->mMaterialIndex >= 0)
    {

        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            if (materials[i].id == mesh_ai->mMaterialIndex){
                mesh->material = materials + i;
                break;
            }
            else if (materials[i].id == -1)
            {
                mesh->material = &materials[i];
                mesh->material->id = mesh_ai->mMaterialIndex;

                aiMaterial* material = scene->mMaterials[mesh_ai->mMaterialIndex];

                unsigned int diffuseNum = material->GetTextureCount(aiTextureType_DIFFUSE);
                unsigned int specularNum = material->GetTextureCount(aiTextureType_SPECULAR);

                if (diffuseNum < 1)
                {
                    std::cout << "diffuseNum " << diffuseNum << std::endl;
                    mesh->material = &materials[0];
                    continue;
                }


                mesh->material->diffuseTextures = (unsigned int*)malloc(diffuseNum * sizeof(unsigned int));
                mesh->material->specularTextures = (unsigned int*)malloc(specularNum * sizeof(unsigned int));

                std::cout << "Load new Material " << mesh_ai->mMaterialIndex << std::endl;

                for (unsigned int j = 0; j < diffuseNum; j++)
                {
                    aiString texturePath;

                    material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath);
                    std::string path = directory + "/" + texturePath.C_Str();
                    std::cout << path << std::endl;
                    loadTexture(mesh->material->diffuseTextures + j, path.c_str(), false);
                }


                for (unsigned int j = 0; j < specularNum; j++)
                {
                    aiString texturePath;

                    material->GetTexture(aiTextureType_SPECULAR, j, &texturePath);
                    std::string path = directory + "/" + texturePath.C_Str();
                    std::cout << path << std::endl;
                    loadTexture(mesh->material->specularTextures + j, path.c_str(), false);
                }
               
                break;
            }
        }
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticess.size() * sizeof(Vertex), &verticess[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicess.size() * sizeof(unsigned int), &indicess[0], GL_STATIC_DRAW);


    mesh->vao = VAO;
    mesh->numIndices = indicess.size();
}

void drawMesh(Shader &shader, Mesh *mesh)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->material->diffuseTextures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mesh->material->specularTextures[0]);
    shader.setInt("material.texture_diffuse1", 0);
    shader.setInt("material.texture_specular1", 0);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
}

void loadTexture(unsigned int *textureMap, const char* path, bool flip)
{
    glGenTextures(1, textureMap);
    glBindTexture(GL_TEXTURE_2D, *textureMap);
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
        return;
    }
    stbi_image_free(data);
}