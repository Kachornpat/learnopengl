#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <glad/glad.h>

#include <iostream>
#include "mesh.h"
#include "shader.h"

void genMesh(const aiScene* scene, aiMesh *mesh_ai, Mesh* mesh, Material* materials, unsigned int numMaterial)
{

    Vertex* vertices = (Vertex*)malloc(mesh_ai->mNumVertices * sizeof(Vertex));
    unsigned int numIndices = 3 * mesh_ai->mNumFaces;
    unsigned int* indices = (unsigned int*)malloc(numIndices * sizeof(unsigned int));

    for (unsigned int i = 0; i < mesh_ai->mNumVertices; i++)
    {
        vertices[i].position.x = mesh_ai->mVertices[i].x;
        vertices[i].position.y = mesh_ai->mVertices[i].y;
        vertices[i].position.z = mesh_ai->mVertices[i].z;

        vertices[i].normal.x = mesh_ai->mNormals[i].x;
        vertices[i].normal.y = mesh_ai->mNormals[i].y;
        vertices[i].normal.z = mesh_ai->mNormals[i].z;

        if (mesh_ai->mTextureCoords[0])
        {
            vertices[i].texCoord.x = mesh_ai->mTextureCoords[0][i].x;
            vertices[i].texCoord.y = mesh_ai->mTextureCoords[0][i].y;
        }
        else
            vertices[i].texCoord = glm::vec2(0.0f, 0.0f);
    }

    unsigned int indexIndice = 0;
    for (unsigned int i = 0; i < mesh_ai->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh_ai->mFaces[i].mNumIndices; j++)
        {
            indices[indexIndice] = mesh_ai->mFaces[i].mIndices[j];
            indexIndice++;
        }
    }

    // ========================================
    if (mesh_ai->mMaterialIndex >= 0)
    {

        for (unsigned int i = 0; i < numMaterial; i++)
        {
            if (materials[i].id == mesh_ai->mMaterialIndex){
                std::cout << "Load existed Material " << std::endl;
                mesh->material = &materials[i];
                break;
            }
            else if(materials[i].id == -1)
            {
                std::cout << "Load new Material " << mesh_ai->mMaterialIndex << std::endl;
                materials[i].id = mesh_ai->mMaterialIndex;
                mesh->material = &materials[i];
                break;
            }
        }
    }
    // ========================================

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  mesh_ai->mNumVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    free(vertices);
    free(indices);

    mesh->vao = VAO;
    mesh->numIndices = numIndices;
}

void drawMesh(Shader &shader, Mesh *mesh)
{
    shader.setInt("material.texture_diffuse1", 0);
    shader.setInt("material.texture_specular1", 1);
    shader.setFloat("material.shininess", 32.0f);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
}
void loadTexture();