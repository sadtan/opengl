#pragma once

#define CHUNK_COL_HEIGHT 16

#include <pch.hpp>
#include <chunk.h>
#include <shader.h>

class ChunkCol
{
public:
    std::vector<Chunk> chunkCol;
    std::vector<Vertex> chunkColVertexList;
    glm::vec3 playerPosition;
    Shader *cubeShader;

    unsigned int myTexture;
    ChunkCol(int x, int z)
    {
        // texture 1
        // ---------
        glGenTextures(1, &myTexture);
        glBindTexture(GL_TEXTURE_2D, myTexture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load(getFilePath("\\resources\\textures\\texture_atlas_1x3.jpg").c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        cubeShader = new Shader(getFilePath("\\shaders\\cube_shader.vs").c_str(), getFilePath("\\shaders\\cube_shader.fs").c_str());
        playerPosition = glm::vec3(0.0f);

        for (int h = 0; h < CHUNK_COL_HEIGHT; ++h)
        {
            Chunk newChunk(myTexture, glm::vec3(x, h, z));
            newChunk.Init();
            chunkCol.push_back(newChunk);
        }

        cubeShader->use();
        cubeShader->setInt("myTexture", 0);
        
    };
    void Draw(glm::mat4 &projection,
              glm::mat4 &model,
              glm::mat4 &view)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, myTexture);

        cubeShader->use();
        cubeShader->setMat4("projection", projection);
        cubeShader->setMat4("view", view);
        cubeShader->setMat4("model", model);

        for (int h = 0; h < CHUNK_COL_HEIGHT; ++h)
        {
            chunkCol[h].Draw(projection, model, view);
        }
    }

    void Clean()
    {
        for (int h = 0; h < CHUNK_COL_HEIGHT; ++h)
        {
            chunkCol[h].Clean();
        }
    }
};