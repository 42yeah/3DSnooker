//
//  StandardProgram.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "StandardProgram.hpp"
#include <fstream>
#include <sstream>
#include "Model.hpp"


void StandardProgram::link(std::string vertexShaderPath, std::string fragmentShaderPath) {
    GLuint program = glCreateProgram();
    glAttachShader(program, compile(GL_VERTEX_SHADER, vertexShaderPath));
    glAttachShader(program, compile(GL_FRAGMENT_SHADER, fragmentShaderPath));
    glLinkProgram(program);
    char log[512];
    glGetProgramInfoLog(program, sizeof(log), nullptr, log);
    std::cout << "Program: " << log << std::endl;
    this->program = program;
    
    // === INITIALIZE LOCATION VARIABLES === //
    this->modelLoc = glGetUniformLocation(this->program, "model");
    this->viewLoc = glGetUniformLocation(this->program, "view");
    this->perspectiveLoc = glGetUniformLocation(this->program, "perspective");
    this->texturizeLoc = glGetUniformLocation(this->program, "texturize");
    this->ambientLoc = glGetUniformLocation(this->program, "ambientTexture");
//    this->diffuseLoc = glGetUniformLocation(this->program, "diffuseTexture");
//    this->specularLoc = glGetUniformLocation(this->program, "specularTexture");
    
    this->timeLoc = glGetUniformLocation(this->program, "time");
}

GLuint StandardProgram::compile(GLuint shaderType, std::string shaderPath) { 
    GLuint shader = glCreateShader(shaderType);
    std::ifstream reader(shaderPath.c_str());
    std::stringstream ss;
    reader >> ss.rdbuf();
    std::string source = ss.str();
    const char *raw = source.c_str();
    glShaderSource(shader, 1, &raw, nullptr);
    glCompileShader(shader);
    char log[512] = { 0 };
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    std::cout << shaderPath.c_str() << ": " << log << std::endl;
    return shader;
}

void StandardProgram::use() {
    glUseProgram(this->program);
}

void StandardProgram::applyM(glm::mat4 model) {
    this->use();
    glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void StandardProgram::applyVP(glm::mat4 view, glm::mat4 perspec) {
    this->use();
    glUniformMatrix4fv(this->viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(this->perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspec));
}

void StandardProgram::applyTexture(Texture *ambient, Texture *diffuse, Texture *specular) {
    this->use();
    if (ambient->isValid()) {
        glUniform1i(this->texturizeLoc, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ambient->glTexture);
    }
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, ambient->glTexture);
//    glUniform1i(this->diffuseLoc, 1);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, ambient->glTexture);
//    glUniform1i(this->specularLoc, 2);
}


