#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>

#include "mesh.hpp"

Mesh loadObj(const std::string &file_path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str()))
    {
        std::cerr << "Failed to load obj file \"" << file_path << "\" due to error: " << err << std::endl;
        return Mesh{};
    }

    if (!warn.empty())
    {
        std::cout << "WARN while loading obj file \"" << file_path << "\": " << warn << std::endl;
    }

    std::vector<Vertex> vertices;

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex = {};

            // Read the data for a vertex from the "attrib" object
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]};

            vertex.tex_coord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0 - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {
                (uint8_t)(attrib.colors[3 * index.vertex_index + 0] * 255),
                (uint8_t)(attrib.colors[3 * index.vertex_index + 1] * 255),
                (uint8_t)(attrib.colors[3 * index.vertex_index + 2] * 255),
                255};

            vertices.push_back(vertex);
        }
    }

    return Mesh::create(vertices);
}

Mesh createPlane()
{
    std::vector<Vertex> vertices = {
        // position                 color            tex_coord            normal
        {{-0.5f, 0.0f, 0.5f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.0f, 0.5f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.0f, -0.5f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        {{0.5f, 0.0f, -0.5f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.0f, -0.5f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.0f, 0.5f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    };

    return Mesh::create(vertices);
}