#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "color.hpp"

struct Vertex
{
    glm::vec3 position;
    Color color;
    glm::vec2 tex_coord;
    glm::vec3 normal;
};

#define ATTRIB_POSITION 0
#define ATTRIB_COLOR 1
#define ATTRIB_TEXCOORD 2
#define ATTRIB_NORMAL 3

struct Mesh
{
    GLuint vao, vbo;
    GLsizei vertex_count;

    static Mesh create(const std::vector<Vertex> &vertices)
    {
        Mesh mesh{};
        mesh.vertex_count = (GLsizei)vertices.size();

        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        glGenBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(ATTRIB_POSITION);
        glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(ATTRIB_COLOR);
        glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));
        glEnableVertexAttribArray(ATTRIB_TEXCOORD);
        glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
        glEnableVertexAttribArray(ATTRIB_NORMAL);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        glBindVertexArray(0);
        return mesh;
    }
    static Mesh load_obj(const std::string &path);
    void draw()
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

    void destroy()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
};
Mesh loadObj(const std::string& file_path);
Mesh createPlane();