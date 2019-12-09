//
//  Snooker.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Snooker.hpp"
#include "../Ext/glad/glad.h"
#include "../Ext/glm/glm.hpp"
#include "../Ext/glm/gtc/matrix_transform.hpp"
#include "../Ext/glm/gtc/type_ptr.hpp"
#include "Model.hpp"


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(testTriangleData), testTriangleData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    // === LET'S LOAD SUZZANE === //
    Model suzzane("Assets/suzzane.obj", "Assets");
    suzzane.load();
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

void Snooker::applyRegularCamera() { 
    view = glm::lookAt(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    perspective = glm::perspective(glm::radians(45.0f),
                                   windowWrapper->getFrameBufferSize().x / windowWrapper->getFrameBufferSize().y,
                                   0.01f,
                                   200.0f);

    // === APPLY TO TEST SHADER === //
    testTriangleProgram.applyMVP(glm::mat4(1.0f), view, perspective);
}

