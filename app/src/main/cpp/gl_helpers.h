//
// Created by 周昊 on 2019/11/2.
//

#ifndef INC_3DSNOOKER_GL_HELPERS_H
#define INC_3DSNOOKER_GL_HELPERS_H

#include <android/log.h>
#include <GLES2/gl2.h>
#include <iostream>
#include "glm/glm.hpp"


// === MACROS === //
#define LOG(...) (__android_log_print(ANDROID_LOG_DEBUG, "Native", __VA_ARGS__))

// === SHADERS === //
class Program;
class Memory;

// === DECLARATIONS === //
class Resources;

// === METHODS === //
Program linkProgram(std::string vSrc, std::string fSrc);

Program linkProgramByPath(std::string vPath, std::string fPath, Resources *res);

GLuint compileShader(GLuint type, std::string src);

void configureCamera(Program &prog, glm::mat4 viewMat, glm::mat4 perspectiveMat);

class Program {
public:
    // === CONSTURCTORS === //
    Program() = default;
    Program(GLuint prog);

    // === METHODS === //
    void init();

    void use();

    // === VARIABLES === //
    GLuint prog;
    GLuint aPosLocation, aColorLocation, aUVLocation;
    GLuint viewLocation, perspectiveLocation, modelLocation;
};

#endif //INC_3DSNOOKER_GL_HELPERS_H
