#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <shader.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\0";
const char *fragmentShaderSourceUniform = "#version 330 core\n"
										  "out vec4 FragColor;\n"
										  "uniform vec4 ourColor;\n"
										  "void main()\n"
										  "{\n"
										  "FragColor = ourColor;\n"
										  "}\0";
const char *vertexShaderSourceColored = "#version 330 core\n"
										"layout(location = 0) in vec3 vertex_position;\n"
										"layout(location = 1) in vec3 vertex_colour;\n"
										"out vec3 colour;\n"
										"void main()\n"
										"{\n"
										"    colour = vertex_colour;\n"
										"    gl_Position = vec4(vertex_position, 1.0);\n"
										"}\0";
const char *fragmentShaderSourceColored = "#version 330 core\n"
										  "in vec3 colour;\n"
										  "out vec4 frag_colour;\n"
										  "void main()\n"
										  "{\n"
										  "    frag_colour = vec4(colour, 1.0);\n"
										  "}\0";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	// -----------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	//
	// vertex shader
	// -----------
	unsigned int vertexShaderColored;
	vertexShaderColored = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColored, 1, &vertexShaderSourceColored, NULL);
	glCompileShader(vertexShaderColored);
	glGetShaderiv(vertexShaderColored, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderColored, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX_SHADER_COLORED::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMEN::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Fragment shader uniform
	unsigned int fragmentShaderUniform;
	fragmentShaderUniform = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderUniform, 1, &fragmentShaderSourceUniform, NULL);
	glCompileShader(fragmentShaderUniform);
	glGetShaderiv(fragmentShaderUniform, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderUniform, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_UNIFORM::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader colored
	unsigned int fragmentShaderColored;
	fragmentShaderColored = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColored, 1, &fragmentShaderSourceColored, NULL);
	glCompileShader(fragmentShaderColored);
	glGetShaderiv(fragmentShaderColored, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderColored, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_COLORED::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// shader program
	// -----------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int successS;
	char infoLogS[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successS);
	if (!successS)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogS);
	}
	//
	unsigned int shaderProgramUniform;
	shaderProgramUniform = glCreateProgram();
	glAttachShader(shaderProgramUniform, vertexShader);
	glAttachShader(shaderProgramUniform, fragmentShaderUniform);
	glLinkProgram(shaderProgramUniform);
	glGetProgramiv(shaderProgramUniform, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramUniform, 512, NULL, infoLog);
	}

	// std::u8string srcPath = std::filesystem::current_path().u8string() + "\\src";
	Shader coloredShader("C:/Dev/cpp/shaders/shader.vs", "C:/Dev/cpp/shaders/shader.fs");

	// COLORED SHADER PROGRAM
	// -------
	unsigned int shaderProgramColored;
	shaderProgramColored = glCreateProgram();
	glAttachShader(shaderProgramColored, vertexShaderColored);
	glAttachShader(shaderProgramColored, fragmentShaderColored);
	glLinkProgram(shaderProgramColored);
	glGetProgramiv(shaderProgramColored, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramColored, 512, NULL, infoLog);
	}

	// clear shaders
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColored);
	glDeleteShader(fragmentShader);
	glDeleteShader(shaderProgramUniform);
	glDeleteShader(fragmentShaderColored);

	// Vertices list
	// -----------
	float vertices[] = {
		0.5f, 0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, 0.5f, 0.0f, // bottom left
		-0.5f, -0.5f, 0.0f // top left
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set our vertex attributes pointers
	// Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Colored vertices list
	float points[] = {
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f //
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};
	// Colored VAO, VBO
	unsigned int VBOC, VBOP;
	glGenBuffers(1, &EBO);
	// VBO points SETUP
	glGenBuffers(1, &VBOP);
	glBindBuffer(GL_ARRAY_BUFFER, VBOP);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// VBO colors SETUP
	glGenBuffers(1, &VBOC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// VAO
	unsigned VAOC;
	glGenVertexArrays(1, &VAOC);
	glBindVertexArray(VAOC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOP);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW MESH
		// USING VERTEX OBJECTS

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgramUniform, "ourColor");
		glUseProgram(shaderProgramUniform);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 1, 4);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// DRAW COLORED MESH
		// glUseProgram(shaderProgramColored);
		glBindVertexArray(VAOC);
		coloredShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glBindVertexArray(1);

		// USING ELEMENT BUFFER OBJECT (EBO)
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFWresources.
	//---------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}