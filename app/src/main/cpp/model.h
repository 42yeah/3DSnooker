//
// Created by 周昊 on 2019/11/2.
//

#ifndef INC_3DSNOOKER_MODEL_H
#define INC_3DSNOOKER_MODEL_H

#include <iostream>
#include <vector>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "files.h"
#include "gl_helpers.h"


// === CONSTANTS === //
const int AXISES = 6;

// === DECLARATIONS === //
class Memory;

class Model {
public:
    // === CONSTRUCTORS === //
    Model() = default;
    Model(Resources *res, std::string path);
    Model(float *buffer, int n);

    // === METHODS === //
    void importFromFile(Resources *res, std::string path);

    void render(Program &prog, Memory &mem);

    void setRenderWireframe(bool wireframe);

    // === PUBLIC VARIABLES === //
    glm::mat4 modelMat;

private:
    // === PRIVATE VARIABLES === //
    int numVertices;
    std::vector<float> vertices;
    int numIndices;
    std::vector<unsigned int> indices;
    bool renderWireframe;
};

class Entity {
public:
    // === CONSTRUCTORS === //
    Entity(Model &model);

    // === METHODS === //
    void update(float dt);

    void render(Program &prog, Memory &mem);

    // === VARIABLES === //
    Model *flyweight;
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 accerleration;
    bool renderWireframe;
};

#endif //INC_3DSNOOKER_MODEL_H
