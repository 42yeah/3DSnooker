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
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;
    
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, modelPath.c_str(), modelMtlBaseDir.c_str());
    std::cout << "Object " << modelName << ": #vertices(" << attributes.vertices.size() << "), #texCoords(" << attributes.texcoords.size() << "), #materials(" << materials.size() << "), #normals(" << attributes.normals.size() << ")" << std::endl;
    for (int i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t shape = shapes[i];
        tinyobj::mesh_t mesh = shape.mesh;
        // TODO: Model loading right here
        std::cout << "SHAPE: " << shapes[i].name << std::endl;
    }
    std::cout << "Model loading done: " << warnings << ", " << errors << std::endl;
}

