#pragma once

#include <string>
#include <glad/gl.h>
#include "color.hpp"

GLuint loadImage(const std::string& image_path);
GLuint fromColor(Color color);