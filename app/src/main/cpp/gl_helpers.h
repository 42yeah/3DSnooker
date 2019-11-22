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
glm::vec3 operator+(glm::vec3 a, glm::vec3 b);

Program linkProgram(std::string vSrc, std::string fSrc);

Program linkProgramByPath(std::string vPath, std::string fPath, Resources *res);

GLuint compileShader(GLuint type, std::string src);

glm::mat4 getViewMatrixByOFU(glm::vec3 origin, glm::vec3 front, glm::vec3 up);

glm::vec3 getViewSphere(float pitch, float yaw);

class Program {
public:
    // === CONSTRUCTORS === //
    Program() = default;
    Program(GLuint prog);

    // === METHODS === //
    void init();

    void use();

    void configureCamera(glm::mat4 viewMat, glm::mat4 perspectiveMat);

    // === VARIABLES === //
    GLuint prog;
    GLuint aPosLocation, aColorLocation, aUVLocation;
    GLuint viewLocation, perspectiveLocation, modelLocation;
    GLuint centerLocation;
};

#endif //INC_3DSNOOKER_GL_HELPERS_H
