//
// Created by 周昊 on 2019/11/2.
//

#ifndef INC_3DSNOOKER_MEMORY_H
#define INC_3DSNOOKER_MEMORY_H

#include <iostream>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.h"
#include "gl_helpers.h"

#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"


class Program;

class Memory {
public:
    // === SOURCES === //
    std::string vertexSource;
    std::string fragmentSource;

    // === OPENGL STUFFS === //
    Program poolProgram;
    Program ballProgram;
    float aspectRatio;
    glm::mat4 viewMat, perspectiveMat;
    glm::vec3 origin, front, up;
    float originX, originY;
    float deltaTime;
    long long int lastInstant;
    float yaw, pitch;
    float yawBase, pitchBase;

    // === MODELS === //
    Model pool;
    Model ball;
    Model hole;

    // === ENTITIES === //
    std::vector<Entity> holes;
    std::vector<glm::vec3> holePositions;

    // === TEST VARIABLES === //
    Model testModel;
    float rotationDegree;

    // === NUKLEAR === //

};

#endif //INC_3DSNOOKER_MEMORY_H
