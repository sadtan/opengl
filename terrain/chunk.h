#pragma once

#include <cube.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

class Chunk
{
private:
    /* data */
    // std::vector
    // Shader *cubeShader;
    unsigned int chunkVBO, chunkVAO;
    unsigned int myTexture;
    std::vector<Vertex> chunkVertices;
    float *chunkVerticesArr;
    glm::vec3 chunkKey;
public:
    std::vector<std::vector<std::vector<Cube>>> Cubes;

    Chunk(unsigned int &texture, glm::vec3 _chunkKey)
    {
        chunkKey = _chunkKey;

        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            // cubesArr[x] = new Cube[COLS];
            std::vector<std::vector<Cube>> v2d;
            pos.x = x + chunkKey.x * CHUNK_SIZE_X;
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                std::vector<Cube> v1d;
                // Set altitude
                pos.y = y + chunkKey.y * CHUNK_SIZE_Y;
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    pos.z = z + chunkKey.z * CHUNK_SIZE_Z;

                    Cube cube(pos);
                    v1d.push_back(cube);
                }
                v2d.push_back(v1d);
            }
            Cubes.push_back(v2d);
        }

        Optimize();

    }

    void Optimize(Chunk *neighborChunks = nullptr)
    {
        for (int x = 0; x < CHUNK_SIZE_X; x++)
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    // Check for intersecting faces
                    int visibleFaces[6] = {1, 1, 1, 1, 1, 1};
                    // TOP FACE
                    visibleFaces[1] = GetBlockType(GetBlockPosition(glm::vec3(x + 1, y,     z    ))); // Cubes[x + 1][y][z].GetType() != AIR ? 0 : 1;
                    visibleFaces[3] = GetBlockType(GetBlockPosition(glm::vec3(x - 1, y,     z    ))); // Cubes[x - 1][y][z].GetType() != AIR ? 0 : 1;
                    visibleFaces[4] = GetBlockType(GetBlockPosition(glm::vec3(x,     y + 1, z    ))); // Cubes[x][y + 1][z].GetType() != AIR ? 0 : 1;
                    visibleFaces[5] = GetBlockType(GetBlockPosition(glm::vec3(x,     y - 1, z    ))); // Cubes[x][y - 1][z].GetType() != AIR ? 0 : 1;
                    visibleFaces[0] = GetBlockType(GetBlockPosition(glm::vec3(x,     y,     z + 1))); // Cubes[x][y][z + 1].GetType() != AIR ? 0 : 1;
                    visibleFaces[2] = GetBlockType(GetBlockPosition(glm::vec3(x,     y,     z - 1))); // Cubes[x][y][z - 1].GetType() != AIR ? 0 : 1;

                    Cubes[x][y][z].UpdateFaces(visibleFaces);
                    Cubes[x][y][z].InitTexture(myTexture);


                    for (int i = 0; i < Cubes[x][y][z].visibleFacesCount * 6; ++i)
                    {
                        // if (visibleFaces[0] == 1)
                        // {

                        // }
                        Vertex vertex;
                        vertex.Position[0] = Cubes[x][y][z].vertices[i * 5 + 0];
                        vertex.Position[1] = Cubes[x][y][z].vertices[i * 5 + 1];
                        vertex.Position[2] = Cubes[x][y][z].vertices[i * 5 + 2];

                        vertex.TexCord[0] = Cubes[x][y][z].vertices[i * 5 + 3];
                        vertex.TexCord[1] = Cubes[x][y][z].vertices[i * 5 + 4];

                        chunkVertices.push_back(vertex);
                    }
                    Cubes[x][y][z].Clean();
                }

        chunkVerticesArr = new float[chunkVertices.size() * 5];
    }

    glm::vec3 GetBlockPosition(glm::vec3 pos)
    {
        return glm::vec3(pos.x + chunkKey.x * CHUNK_SIZE_X, pos.y + chunkKey.y * CHUNK_SIZE_Y, pos.z + chunkKey.z * CHUNK_SIZE_Z);
    }

    void Init() 
    {

        if (chunkVertices.size() > 0)
        {
            memcpy(chunkVerticesArr, chunkVertices.data(), chunkVertices.size() * sizeof(Vertex));

            glGenVertexArrays(1, &chunkVAO);
            glBindVertexArray(chunkVAO);

            // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 180, vertices, GL_STATIC_DRAW);

            glGenBuffers(1, &chunkVBO);
            glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * chunkVertices.size(), chunkVerticesArr, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
            // color attribute

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCord));
        }
    }

    void Draw(glm::mat4 &projection,
              glm::mat4 &model,
              glm::mat4 &view)
    {
        if (chunkVertices.size() > 0)
        {

            glBindVertexArray(chunkVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36 * chunkVertices.size());
        }
    }

    void Clean()
    {
        glDeleteVertexArrays(1, &chunkVAO);
        glDeleteBuffers(1, &chunkVBO);

    }

    // Chunk::~Chunk()
    // {
    //     // delete[]
    // }
};
