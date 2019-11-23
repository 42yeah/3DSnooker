//
//  Snooker.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Snooker.hpp"
#include "../Ext/glad/glad.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../Ext/tiny_obj_loader.h"


Snooker::Snooker(WindowWrapper *wrapper) : windowWrapper(wrapper) {
    
}

void Snooker::update() {
    
}

void Snooker::init() {
    globalRotation = glm::mat4(1.0f);
    
    // === POOL TABLE === //

    
    // === TEST DATA === //
    testTriangleProgram.link("Assets/testTriangle.vertex.glsl",
                             "Assets/testTriangle.fragment.glsl");
    
    glGenVertexArrays(1, &testTriangleVAO);
    glGenBuffers(1, &testTriangleVBO);
    glBindVertexArray(testTriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testTriangleVBO);
    float testTriangleData[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(testTriangleData), testData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    
    glGenVertexArrays(1, &testCubeVAO);
    GLuint testCubeVBO;
    glGenBuffers(1, &testTriangleVBO);
    glBindVertexArray(testCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testCubeVBO);
    float testCubeData[] = {
        
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(testCubeData), testData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;
    tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, "Assets/suzzane.obj", "Assets");
    for (int i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t shape = shapes[i];
        std::cout << shapes[i] << std::endl;
    }
    std::cout << "Model loading done: " << warnings << ", " << errors << std::endl;
}

void Snooker::renderSkybox() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Snooker::renderTable() {
    
}

void Snooker::renderHoles() {
    
}

void Snooker::renderBalls() {
    
}

void Snooker::renderCueStick() {
    
}

void Snooker::renderTestTriangle() { 
    testTriangleProgram.use();
    glBindVertexArray(testTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Snooker::renderTestSuzzane() {
    
}

void Snooker::renderTestCube() {
    
}
