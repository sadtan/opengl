#pragma once

#include <pch.hpp>
#include <chunk_col.h>
#define GENERATION_RADIOUS 1

class TerrainGenerator
{
public:
    std::vector<ChunkCol> loadedChunkCol;

    TerrainGenerator()
    {
        for (int x = -GENERATION_RADIOUS; x < GENERATION_RADIOUS; ++x)
        {
            for (int z = -GENERATION_RADIOUS; z < GENERATION_RADIOUS; ++z)
            {
                ChunkCol chunkCol(x, z);
                loadedChunkCol.push_back(chunkCol);
            }
        }
    }

    void Draw(glm::mat4 &projection,
              glm::mat4 &model,
              glm::mat4 &view)
    {
        for (int c = 0; c < loadedChunkCol.size(); ++c)
        {
            loadedChunkCol[c].Draw(projection, model, view);
        }
    }

    void Clean()
    {
        for (int c = 0; c < loadedChunkCol.size(); ++c)
        {
            loadedChunkCol[c].Clean();
        }
    }
};