//
//  Model.cpp
//  Snooker3D
//
//  Created by apple on 2019/12/9.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Model.hpp"
#include "../Ext/glad/glad.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../Ext/tiny_obj_loader.h"


Model::Model(std::string path, std::string mtlBaseDir) {
    this->modelPath = path;
    this->modelMtlBaseDir = mtlBaseDir;
    this->modelName = path;
}

void Model::load() {
    this->numVertices = 0;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;

    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, modelPath.c_str(), modelMtlBaseDir.c_str());

    std::vector<Vertex> vertices;
    for (int i = 0; i < shapes.size(); i ++) {
        tinyobj::shape_t &shape = shapes[i];
        tinyobj::mesh_t &mesh = shape.mesh;
        std::cout << "# indices: " << mesh.indices.size() << std::endl;
        for (int j = 0; j < mesh.indices.size(); j++) {
            tinyobj::index_t i = mesh.indices[j];
            glm::vec3 position = {
                attributes.vertices[i.vertex_index * 3],
                attributes.vertices[i.vertex_index * 3 + 1],
                attributes.vertices[i.vertex_index * 3 + 2]
            };
            std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
            glm::vec3 normal = {
                attributes.vertices[i.normal_index * 3],
                attributes.vertices[i.normal_index * 3 + 1],
                attributes.vertices[i.normal_index * 3 + 2]
            };
            glm::vec2 texCoord = {
                attributes.vertices[i.texcoord_index * 2],
                attributes.vertices[i.texcoord_index * 2 + 1],
//                attributes.vertices[i2.texcoord_index]
            };
            // Not gonna care about texCoord right now.
            Vertex vert = { position, normal, texCoord };
            vertices.push_back(vert);
        }
        this->numVertices += mesh.indices.size();
    }
    
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 6));
    this->VAO = VAO;
    std::cout << "Model loading done: " << warnings << ", " << errors << ". " << std::endl;
}

void Model::render(StandardProgram &program) { 
    program.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

