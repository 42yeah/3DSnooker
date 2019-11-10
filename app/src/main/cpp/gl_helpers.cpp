//
// Created by 周昊 on 2019/11/2.
//

#include "gl_helpers.h"
#include <utility>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "files.h"


glm::vec3 operator+(glm::vec3 a, glm::vec3 b) {
    return glm::vec3(
            a.x + b.x,
            a.y + b.y,
            a.z + b.z
    );
}

Program linkProgram(std::string vSrc, std::string fSrc) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, compileShader(GL_VERTEX_SHADER, std::move(vSrc)));
    glAttachShader(prog, compileShader(GL_FRAGMENT_SHADER, std::move(fSrc)));
    char log[512] = { 0 };
    glLinkProgram(prog);
    glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
    LOG("Program: %s", log);
    Program program(prog);
    return program;
}

GLuint compileShader(GLuint type, std::string src) {
    GLuint shader = glCreateShader(type);
    const char *raw = src.c_str();
    glShaderSource(shader, 1, &raw, nullptr);
    glCompileShader(shader);
    char log[512] = { 0 };
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    LOG("Shader: %s", log);
    return shader;
}

Program linkProgramByPath(std::string vPath, std::string fPath, Resources *res) {
    std::string vSrc = res->readFileAsString(vPath);
    std::string fSrc = res->readFileAsString(fPath);
    Program program = linkProgram(
            vSrc,
            fSrc
    );
    return program;
}

glm::mat4 getViewMatrixByOFU(glm::vec3 origin, glm::vec3 front, glm::vec3 up) {
    return glm::lookAt(
            origin,
            origin + front,
            up
    );
}

glm::vec3 getViewSphere(float pitch, float yaw) {
    float x = sinf(yaw) * 5.0f;
    float y = sinf(pitch) * 5.0f;
    float z = cosf(yaw) * cosf(pitch) * 5.0f;
    return glm::vec3(x, y, z);
}

// === PROGRAM === //
// === CONSTRUCTORS === //
Program::Program(GLuint prog) {
    this->prog = prog;
    init();
}

// === METHODS === //
void Program::init() {
    this->aPosLocation = (GLuint) glGetAttribLocation(
            prog,
            "aPos"
    );
    this->aColorLocation = (GLuint) glGetAttribLocation(
            prog,
            "aColor"
    );
    this->aUVLocation = (GLuint) glGetAttribLocation(
            prog,
            "aUV"
    );
    this->perspectiveLocation = (GLuint) glGetUniformLocation(
            prog,
            "perspective"
    );
    this->viewLocation = (GLuint) glGetUniformLocation(
            prog,
            "view"
    );
    this->modelLocation = (GLuint) glGetUniformLocation(
            prog,
            "model"
    );
}

void Program::use() {
    glUseProgram(prog);
}

void Program::configureCamera(glm::mat4 viewMat, glm::mat4 perspectiveMat) {
    glUniformMatrix4fv(
            this->viewLocation,
            1,
            GL_FALSE,
            glm::value_ptr(viewMat)
    );
    glUniformMatrix4fv(
            this->perspectiveLocation,
            1,
            GL_FALSE,
            glm::value_ptr(perspectiveMat)
    );
}
