#pragma once

#include <pch.hpp>

enum Block_Type
{
    GRASS, AIR, DIRT
};

struct Vertex 
{
    float Position[3];
    float TexCord[2];
};

Block_Type GetBlockType(glm::vec3 pos)
{
    // srand ((x + 1) * (z + 1));
    // int surface = 100 + rand() % 20;

    // return y < surface ? GRASS : AIR;
    float frequency = 0.1f;
    float altitude  = 10;
    int xOffset = glm::sin(pos.x * frequency) * altitude;
    int zOffset = glm::sin(pos.z * frequency) * altitude;
    
    int surface = 100 + xOffset + zOffset;
    int seaLevel = 12;

    if (pos.y < surface)
        return GRASS;
    else if (pos.y < seaLevel)
        return DIRT;
    else
        return AIR;
}

static float frontFace [30] {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // TOP    RIGHT
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // BOTTOM RIGHT
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // BOTTOM LEFT
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // BOTTOM LEFT
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // TOP    LEFT
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // TOP    RIGHT
};
static float rightFace [30] {
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // TOP    RIGHT
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT BACK
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // BOTTOM RIGHT
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // BOTTOM RIGHT
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT BACK
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT BACK
};
static float backFace [30] {
       1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // TOP    RIGHT BACK //
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    LEFT  BACK //
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM RIGHT BACK //
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM RIGHT BACK //
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    LEFT  BACK //
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // BOTTOM LEFT  BACK //
};
static float leftFace [30] {
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // TOP    LEFT  BACK //
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // TOP    LEFT //
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM LEFT  BACK //
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM LEFT  BACK //
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // TOP    LEFT //
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // BOTTOM LEFT
};
static float topFace [30] {
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // TOP    LEFT
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT BACK //
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // TOP    RIGHT
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT BACK //
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // TOP    LEFT
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // TOP    LEFT  BACK //
};
static float bottomFace [30] {
        0.0f, 0.0f, 1.0f, 0.0f , 1.0f, // BOTTOM LEFT
        1.0f, 0.0f, 1.0f, 1.0f , 1.0f, // BOTTOM RIGHT
        1.0f, 0.0f, 0.0f, 1.0f , 0.0f, // BOTTOM RIGHT BACK //
        1.0f, 0.0f, 0.0f, 1.0f , 0.0f, // BOTTOM RIGHT BACK //
        0.0f, 0.0f, 0.0f, 0.0f , 0.0f, // BOTTOM LEFT  BACK //
        0.0f, 0.0f, 1.0f, 0.0f , 1.0f, // BOTTOM LEFT
};

class Cube
{
public:
    glm::vec3 position;
    Block_Type cubeType;
    float* vertices;
    int visibleFacesCount;
    
    Cube(glm::vec3 position_)
    {
        position = position_;
        cubeType = GetBlockType(position);
        visibleFacesCount = 0;
    }

    std::string GetType_Str() 
    {
        static const char *enum_str[] =
        { "GRASS", "AIR"};
        std::string str = enum_str[cubeType]; 
        return str;
    }

    Block_Type GetType() 
    {
        return cubeType;
    }

    void UpdateFaces(int *visibleFaces)
    {
        if (cubeType != AIR)
        {

            visibleFacesCount = 0;
            for (int i = 0; i < 6; ++i)
            {
                visibleFacesCount += visibleFaces[i];
            }

            int newVerticesSize = visibleFacesCount * 5 * 6;
            vertices = new float[newVerticesSize];

            int offset = 0;

            if (visibleFaces[0] == 1)
            {
                memcpy(vertices, frontFace, 6 * sizeof(Vertex));
                offset++;
            }
            if (visibleFaces[1] == 1)
            {
                memcpy(vertices + offset * 30, rightFace, 6 * sizeof(Vertex));
                offset++;
            }
            if (visibleFaces[2] == 1)
            {
                memcpy(vertices + offset * 30, backFace, 6 * sizeof(Vertex));
                offset++;
            }
            if (visibleFaces[3] == 1)
            {
                memcpy(vertices + offset * 30, leftFace, 6 * sizeof(Vertex));
                offset++;
            }
            if (visibleFaces[4] == 1)
            {
                memcpy(vertices + offset * 30, topFace, 6 * sizeof(Vertex));
                offset++;
            }
            if (visibleFaces[5] == 1)
            {
                memcpy(vertices + offset * 30, bottomFace, 6 * sizeof(Vertex));
            }

            for (int i = 0; i < visibleFacesCount * 6; ++i)
            {
                vertices[i * 5 + 0] += position.x;
                vertices[i * 5 + 1] += position.y;
                vertices[i * 5 + 2] += position.z;
            }
        }
    }

    void InitTexture(unsigned int &texture)
    {
        if (cubeType != AIR)
        {

            float textureW = 16 * 3;
            float textureH = 16 * 1;
            glm::vec2 index(1, 1);

            //
            float stepX = 1.0f / (textureW / 16.0f);
            float stepY = 1.0f / (textureH / 16.0f);

            glm::vec3 faceNormal;
            for (int i = 0; i < visibleFacesCount * 6; ++i)
            {

                // Calculate Face Normal
                // Front face
                if (i % 6 == 0)
                {
                    glm::vec3 v0(vertices[i * 5 + 0],
                                 vertices[i * 5 + 1],
                                 vertices[i * 5 + 2]);
                    glm::vec3 v1(vertices[(i + 1) * 5 + 0],
                                 vertices[(i + 1) * 5 + 1],
                                 vertices[(i + 1) * 5 + 2]);
                    glm::vec3 v2(vertices[(i + 2) * 5 + 0],
                                 vertices[(i + 2) * 5 + 1],
                                 vertices[(i + 2) * 5 + 2]);

                    glm::vec3 U(v1 - v0);
                    glm::vec3 W(v2 - v0);
                    faceNormal = -glm::cross(U, W);
                }

                // Offset texture coords
                // -----------------------

                // Front to left face
                if (cubeType == GRASS)
                {
                    if (glm::length(glm::vec3(faceNormal.x, 0.0f, faceNormal.z)) != 0)
                    {
                        index.x = 3;
                    }
                    else if (faceNormal.y > 0) // Top face
                    {
                        index.x = 1;
                    }
                    else
                    {
                        index.x = 2;
                    }
                }
                if (cubeType == DIRT)
                {
                    index.x = 2;
                }

                float texX = vertices[i * 5 + 3];
                float texY = vertices[i * 5 + 4];

                if (texX == 1.0f)
                    texX = stepX * index.x;
                if (texX == 0.0f)
                    texX = stepX * (index.x - 1);

                texY = texY * (1.0f / (textureH / 16.0f));

                vertices[i * 5 + 3] = texX;
                vertices[i * 5 + 4] = texY;
            }
        }
        
    }

    void Clean()
    {
        delete vertices;

    }
};

// float textureCords[] = {
//     // FRONT FACE
//     1.0f, 1.0f, //
//     1.0f, 0.0f, //
//     0.0f, 0.0f, //
//     0.0f, 0.0f, //
//     0.0f, 1.0f, //
//     1.0f, 1.0f, //
//     // RIGHT FACE
//     0.0f, 1.0f, //
//     1.0f, 1.0f, //
//     0.0f, 0.0f, //
//     0.0f, 0.0f, //
//     1.0f, 1.0f, //
//     1.0f, 0.0f, //
//     // BACK FACE
//     0.0f, 1.0f, //
//     1.0f, 1.0f, //
//     0.0f, 0.0f, //
//     0.0f, 0.0f, //
//     1.0f, 1.0f, //
//     1.0f, 0.0f, //
//     // LEFT FACE
//     0.0f, 1.0f, //
//     1.0f, 1.0f, //
//     0.0f, 0.0f, //
//     0.0f, 0.0f, //
//     1.0f, 1.0f, //
//     1.0f, 0.0f, //
//     // TOP FACE
//     0.0f, 0.0f, //
//     1.0f, 1.0f, //
//     1.0f, 0.0f, //
//     1.0f, 1.0f, //
//     0.0f, 0.0f, //
//     0.0f, 1.0f, //
//     // BOTTOM FACE

//     0.0f, 1.0f,
//     1.0f, 1.0f,
//     1.0f, 0.0f,
//     1.0f, 0.0f,
//     0.0f, 0.0f,
//     0.0f, 1.0f}; // position attribute

// indexed mes initializati
// float vertices[] = {
// 		// positions          // colors           // texture coords
// 		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	   // 0 top right
// 		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 1 bottom right
// 		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 2 bottom left
// 		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // 3 top left
// 		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 4 back bottom left
// 		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // 5 back top left
// 		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // 6 back top right
// 		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f   // 7 back bottom right
// 	};

// 	unsigned int indices[] = {
// 		// Front Face
// 		0, 1, 3, // first triangle
// 		1, 2, 3, // second triangle
// 		// Left Face
// 		3, 2, 5, // first triangle
// 		2, 4, 5, // second triangle
// 		// Back Face
// 		5, 4, 6,
// 		4, 7, 6,
// 		// Right Face
// 		6, 7, 0,
// 		7, 1, 0,
// 		// Top Face
// 		0, 3, 5,
// 		5, 6, 0,
// 		// Bottom Face
// 		4, 1, 7,
// 		4, 2, 1 //
// 	};
// 	glm::vec3 cubePositions[] = {
// 		glm::vec3(0.0f, -1.0f, -1.0f),
// 		glm::vec3(-1.0f, 0.0f, -1.0f),
// 		glm::vec3(-1.0f, 1.0f, -1.0f),
// 		glm::vec3(1.0f, -1.0f, -1.0f),
// 		// glm::vec3(1.0f, -1.0f, -2.0f),
// 		// glm::vec3(1.0f, -1.0f, -3.0f),
// 		// glm::vec3(1.0f, -1.0f, -4.0f),
// 		// glm::vec3(.0f, -1.0f, 2.0f),
// 		// glm::vec3(.0f, -1.0f, 1.0f),
// 		// glm::vec3(.0f, -1.0f, 0.0f),
// 		// glm::vec3(.0f, -1.0f, -1.0f),
// 		// glm::vec3(.0f, -1.0f, -2.0f),
// 		// glm::vec3(.0f, -1.0f, -3.0f),
// 		// glm::vec3(.0f, -1.0f, -4.0f) //
// 	};
// 	unsigned int VBO, VAO, EBO;
// 	glGenVertexArrays(1, &VAO);
// 	glGenBuffers(1, &VBO);
// 	glGenBuffers(1, &EBO);

// 	glBindVertexArray(VAO);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 	// position attribute
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
// 	glEnableVertexAttribArray(0);
// 	// color attribute
// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);
// 	// texture coord attribute
// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
// 	glEnableVertexAttribArray(2);

// MESH RENDERING

// glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
// trans = glm::rotate(trans, glm::radians((float)deltaTime * 0.000005f), glm::vec3(.0f, 1.0f, .0f));
// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
// render container
// Rotate model
// model = glm::rotate(model, glm::radians(45.0f + angle), glm::vec3(1.0f, 0.0f, .0f));
// float radius = 10.0f;
// float camX = static_cast<float>(sin(angle) * radius);
// float camZ = static_cast<float>(cos(angle) * radius);
// view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
// coloredShader.setMat4("view", view);
// angle += (float)deltaTime * 0.00000005f;
// //
// glBindVertexArray(VAO);
// for (unsigned int i = 0; i < 3; i++)
// {
// 	// calculate the model matrix for each object and pass it to shader before drawing
// 	glm::mat4 model = glm::mat4(1.0f);
// 	model = glm::translate(model, cubePositions[i]);
// 	// float angle1 = 20.0f * i;
// 	// model = glm::rotate(model, glm::radians(angle1), glm::vec3(1.0f, 0.3f, 0.5f));
// 	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
// 	coloredShader.setMat4("model", model);
// 	// glDrawArrays(GL_TRIANGLES, 0, 36);
// 	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
// }s

// glDeleteVertexArrays(1, &VAO);
// glDeleteBuffers(1, &VBO);
// glDeleteBuffers(1, &EBO);

// Shader
// Shader coloredShader(getFilePath("\\shaders\\shader.vs").c_str(), getFilePath("\\shaders\\shader.fs").c_str());
// After declaring Texture
// coloredShader.use();
// coloredShader.setInt("myTexture", 0);
// During rendering
// coloredShader.use();
// coloredShader.setMat4("projection", projection);
