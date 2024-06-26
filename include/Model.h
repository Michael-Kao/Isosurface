#pragma once
#include "Isosurface.h"
#include <glad/glad.h>

class Model {
public:
    Model(std::string const &filePath, std::string const &infFile, unsigned int isovalue);
    GLuint vbo;
    glm::vec3 center;
    void draw();
private:
    Isosurface isosurface;
    std::vector<float> data;
    GLuint vao;
};