#pragma once

#include <GL/gl.h>
#include <my_glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Color;
};


struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    glm::vec3 position = glm::vec3(0.0f);
};