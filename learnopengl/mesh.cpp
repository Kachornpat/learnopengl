#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}
            vertice.texCoord = glm::vec2(0.0f, 0.0f);
void Mesh::setupMesh()
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}


void Mesh::draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        shader.setInt(("material." + name + number).c_str(), i);
    }

                for (unsigned int j = 0; j < diffuseNum; j++)
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
                }



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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

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