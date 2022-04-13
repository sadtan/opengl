#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>


std::string getFilePath(std::string filename) {
	std::string filePath = std::filesystem::current_path().concat(filename).string();
	return filePath;
}