#include "Model.h"

Model::Model(std::string const &filePath, std::string const &infFile, unsigned int isovalue) 
    :isosurface(filePath, infFile, isovalue)
{
    data = isosurface.MarchingCube();
    center = isosurface.center;

    size_t vertSize = data.size() * sizeof(float);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertSize, data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Model::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, data.size() / 6);
}