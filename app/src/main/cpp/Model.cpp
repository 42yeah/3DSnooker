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
#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "Ext/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Ext/stb_image.h"
#include "Macros.hpp"


Model::Model(std::string objPath, std::string mtlPath, std::string baseDir) {
    this->modelPath = objPath;
    this->modelMtlPath = mtlPath;
    this->modelName = objPath;
    this->textureBaseDir = baseDir;
    this->modelMatrix = glm::mat4(1.0f);
}

void Model::load(Resources *loader, int index, TextureStore *store) {
    this->numVertices = 0;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;

    std::stringstream objStream = loader->readFileAsStringStream(modelPath);
    std::stringstream mtlStringStream = loader->readFileAsStringStream(modelMtlPath);
    std::istream &mtlInputStream = mtlStringStream;
    tinyobj::MaterialStreamReader mtlStream(mtlInputStream);
    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, &objStream, &mtlStream);
//    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, modelPath.c_str(), modelMtlBaseDir.c_str());

    if (materials.size() > 0) {
        // There IS texture!
        tinyobj::material_t &material = materials[0];
        ambientTexture = store->load(textureBaseDir + "/" + material.diffuse_texname);
        diffuseTexture = store->load(textureBaseDir + "/" + material.diffuse_texname);
        specularTexture = store->load(textureBaseDir + "/" + material.specular_texname);
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
        LOG("Processing: %s, shape #%d, indices #%u", modelName.c_str(), i, mesh.indices.size());
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

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    this->VBO = VBO;

    LOG("Model loading done: %s, %s. #vertices: %u", warnings.c_str(), errors.c_str(), vertices.size());
}

void Model::render(StandardProgram &program) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    program.configVertexPointers();
    program.applyM(modelMatrix);
    program.applyTexture(ambientTexture, diffuseTexture, specularTexture);
    program.use();

    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

Texture::Texture(std::string filename) {
    this->filename = filename;
    this->glTexture = 0;
    this->valid = false;
}

void Texture::load(Resources *loader) {
    stbi_set_flip_vertically_on_load(true);
    std::string data = loader->readFileAsString(filename);
    if (data == "") {
        valid = false;
        LOG("Loading texture %s failed: file invalid.", filename.c_str());
        return;
    }
    unsigned char *raw = stbi_load_from_memory((unsigned char *) data.c_str(), data.size(), &w, &h, &channels, 0);
    if (!raw) {
        valid = false;
        LOG("Loading texture %s failed: format invalid.", filename.c_str());
        return;
    }
    LOG("Loading texture %s", filename.c_str());
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(raw);
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
    newTexture->load(resourceLoader);
    textures.push_back(newTexture);
    return newTexture;
}

TextureStore::TextureStore(Resources *loader) : resourceLoader(loader) {
}
