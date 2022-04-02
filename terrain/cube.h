#ifndef CUBE_H
#define CUBE_H

#include <pch.h>
#include <shader.h>

std::string getFilePath1(std::string filename)
{
    std::string filePath = std::filesystem::current_path().concat(filename).string();
    return filePath;
}

enum Block_Type {
    GRASS
};

class Cube
{
public:
    glm::vec3 position;
    unsigned int cubeVBO, cubeVAO;
    unsigned int myTexture;
    Shader *cubeShader;

    float vertices[180] = {

        // FRONT FACE
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT
         1.0f,  0.0f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT
         0.0f,  0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM LEFT
         0.0f,  0.0f, 0.0f, 0.0f, 0.0f, // BOTTOM LEFT
         0.0f,  1.0f, 0.0f, 0.0f, 1.0f, // TOP    LEFT
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // TOP    RIGHT

        // RIGHT FACE
         1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // TOP    RIGHT
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    RIGHT BACK
         1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // BOTTOM RIGHT
         1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // BOTTOM RIGHT
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    RIGHT BACK
         1.0f,  0.0f, -1.0f, 1.0f, 0.0f, // BOTTOM RIGHT BACK

        // BACK FACE
         1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // TOP    RIGHT BACK //
         0.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    LEFT  BACK //
         1.0f,  0.0f, -1.0f, 0.0f, 0.0f, // BOTTOM RIGHT BACK //
         1.0f,  0.0f, -1.0f, 0.0f, 0.0f, // BOTTOM RIGHT BACK //
         0.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    LEFT  BACK //
         0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // BOTTOM LEFT  BACK //

        // LEFT FACE
         0.0f,  1.0f, -1.0f, 0.0f, 1.0f, // TOP    LEFT  BACK //
         0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TOP    LEFT //
         0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // BOTTOM LEFT  BACK //
         0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // BOTTOM LEFT  BACK //
         0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TOP    LEFT //
         0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // BOTTOM LEFT

        // TOP FACE
         0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // TOP    LEFT
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    RIGHT BACK //
         1.0f,  1.0f,  0.0f, 1.0f, 0.0f, // TOP    RIGHT
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // TOP    RIGHT BACK //
         0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // TOP    LEFT          
         0.0f,  1.0f, -1.0f, 0.0f, 1.0f, // TOP    LEFT  BACK //

        // BOTTOM FACE
         0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // BOTTOM LEFT
         1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // BOTTOM RIGHT
         1.0f,  0.0f, -1.0f, 1.0f, 0.0f, // BOTTOM RIGHT BACK //
         1.0f,  0.0f, -1.0f, 1.0f, 0.0f, // BOTTOM RIGHT BACK //
         0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // BOTTOM LEFT  BACK //
         0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // BOTTOM LEFT
    //
    };

    Cube(glm::vec3 position_)
    {
        position = position_;
        cubeShader = new Shader(getFilePath1("\\shaders\\cube_shader.vs").c_str(), getFilePath1("\\shaders\\cube_shader.fs").c_str());
    }

    void Init(unsigned int texture, Block_Type blockType)
    {
        float textureW = 16 * 3;
        float textureH = 16 * 1;
        if (blockType == GRASS) {
            
            // Texture Atlas Position
            glm::vec2 index(3, 1);
            float stepX = 1.0f / (textureW / 16.0f);
            float stepY = 1.0f / (textureH / 16.0f);
            for (int i = 0; i < 36; ++i) // Offset texture coords
            {
                if (i < 6 * 4)
                    index.x = 3; // Grass + Dirt
                else if (i < 6 * 5)
                    index.x = 1; // Grass
                else
                    index.x = 2; // Dirt

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
            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);
            glBindVertexArray(cubeVAO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);
            // color attribute
            
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Texture
            myTexture = texture;
            cubeShader->use();
            cubeShader->setInt("myTexture", 0);
    }

    void Draw(
        glm::mat4 &projection,
        glm::mat4 &model,
        glm::mat4 &view)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, myTexture);
        cubeShader->use();
        cubeShader->setMat4("projection", projection);
        cubeShader->setMat4("view", view);
        glBindVertexArray(cubeVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        cubeShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Clean()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);
    }
};
#endif

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

//glDeleteVertexArrays(1, &VAO);
//glDeleteBuffers(1, &VBO);
//glDeleteBuffers(1, &EBO);


// Shader
//Shader coloredShader(getFilePath("\\shaders\\shader.vs").c_str(), getFilePath("\\shaders\\shader.fs").c_str());
// After declaring Texture
//coloredShader.use();
//coloredShader.setInt("myTexture", 0);
// During rendering
//coloredShader.use();
//coloredShader.setMat4("projection", projection);
