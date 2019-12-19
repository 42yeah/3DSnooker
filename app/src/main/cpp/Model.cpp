//
//  Model.cpp
//  Snooker3D
//
//  Created by apple on 2019/12/9.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Model.hpp"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "Ext/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Ext/stb_image.h"


Model::Model(std::string path, std::string mtlBaseDir) {
    this->modelPath = path;
    this->modelMtlBaseDir = mtlBaseDir;
    this->modelName = path;
    this->modelMatrix = glm::mat4(1.0f);
}

void Model::load(int index, TextureStore *store) {
    this->numVertices = 0;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;

    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, modelPath.c_str(), modelMtlBaseDir.c_str());

    int textureWidth = 0;
    int textureHeight = 0;
    if (materials.size() > 0) {
        // There IS texture!
        tinyobj::material_t &material = materials[0];
        ambientTexture = store->load(modelMtlBaseDir + "/" + material.diffuse_texname);
        textureWidth = ambientTexture->w;
        textureHeight = ambientTexture->h;
        diffuseTexture = store->load(modelMtlBaseDir + "/" + material.diffuse_texname);
        specularTexture = store->load(modelMtlBaseDir + "/" + material.specular_texname);
    }

    std::vector<Vertex> vertices;
    int lowerBound = 0;
    int upperBound = (int) shapes.size();
    if (index > -1) {
        lowerBound = index;
        upperBound = index + 1;
    }
    for (int i = lowerBound; i < upperBound; i++) {
        tinyobj::shape_t &shape = shapes[i];
        tinyobj::mesh_t &mesh = shape.mesh;
        std::cout << "Processing: " << modelName << ", shape #" << i << ", #indices " << mesh.indices.size() << std::endl;
        for (long j = 0; j < mesh.indices.size(); j++) {
            tinyobj::index_t i = mesh.indices[j];
            glm::vec3 position = {
                attributes.vertices[i.vertex_index * 3],
                attributes.vertices[i.vertex_index * 3 + 1],
                attributes.vertices[i.vertex_index * 3 + 2]
            };
            glm::vec3 normal = {
                attributes.normals[i.normal_index * 3],
                attributes.normals[i.normal_index * 3 + 1],
                attributes.normals[i.normal_index * 3 + 2]
            };
            glm::vec2 texCoord = {
//                (float) j / mesh.indices.size(),
//                (float) (1.0f - j / mesh.indices.size()),
                attributes.texcoords[i.texcoord_index * 2],
                attributes.texcoords[i.texcoord_index * 2 + 1],
            };
            Vertex vert = { position, normal, texCoord };
            vertices.push_back(vert);
        }
        this->numVertices += mesh.indices.size();
    }

    // TODO: Fix this
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

    std::cout << "Model loading done: " << warnings << ", " << errors << ". " << std::endl;
}

void Model::render(StandardProgram &program) {
    program.applyM(modelMatrix);
    program.applyTexture(ambientTexture, diffuseTexture, specularTexture);
    program.use();

    // TODO: Fix this
//    glBindVertexArray(VAO);
//    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

Texture::Texture(std::string filename) {
    this->filename = filename;
    this->glTexture = 0;
    this->valid = false;
}

void Texture::load() {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &channels, 0);
    if (data == nullptr) {
        valid = false;
        return;
    }
    std::cout << "Loading texture " << filename.c_str() << std::endl;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    valid = true;
}

bool Texture::isValid() {
    return valid;
}

Texture *TextureStore::load(std::string filename) {
    for (int i = 0; i < textures.size(); i++) {
        if (textures[i]->filename == filename) {
            return textures[i];
        }
    }
    Texture *newTexture = new Texture(filename);
    newTexture->load();
    textures.push_back(newTexture);
    return newTexture;
}
