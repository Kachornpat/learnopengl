#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"

Mesh::Mesh(const aiScene* scene, aiMesh* mesh, std::vector<Material> &materials)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
   
    material = NULL;

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
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    numIndices = indices.size();

    if (mesh->mMaterialIndex >= 0)
    {
        material = &materials[mesh->mMaterialIndex];
    }

    unsigned int VBO, EBO;
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

}

void Mesh::draw(Shader &shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material->specularMap);
    shader.setInt("material.texture_diffuse1", 0);
    shader.setInt("material.texture_specular1", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

