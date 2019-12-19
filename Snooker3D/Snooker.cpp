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
#include <random>


Snooker::Snooker(WindowWrapper *wrapper) : windowWrapper(wrapper) {
    
}

void Snooker::init() {
    globalRotation = glm::mat4(1.0f);
    
    // === POOL TABLE === //

    
    // === TEST DATA === //
    program.link("Assets/testTriangle.vertex.glsl",
                             "Assets/testTriangle.fragment.glsl");
    
    glGenVertexArrays(1, &testTriangleVAO);
    glGenBuffers(1, &testTriangleVBO);
    glBindVertexArray(testTriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testTriangleVBO);
    float testTriangleData[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(testTriangleData), testTriangleData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 6));
    
    glEnable(GL_DEPTH_TEST);

    billiardTable = Model("Assets/PoolTable/PoolTable.obj", "Assets/PoolTable");
    billiardTable.load(-1, &textureStore);
    loadBallModels();
    
    // === TIME === //
    lastInstant = glfwGetTime();
    
    // === ENTITIES === //
    int indices[] = {
        0,
        1, 2, 3, 11, 12, 13, 15,
        4, 5, 6, 7,   8,  9, 14,
        10
    };
    for (int i = 0; i < 16; i++) {
        EntityType entityType;
        if (i == 0) {
            entityType = SELF;
        } else if (i >= 1 && i <= 7) {
            entityType = FRIEND;
        } else if (i >= 8 && i <= 15) {
            entityType = FOE;
        } else {
            entityType = BLACK;
        }
        Entity ball(entityType, &ballModels[indices[i]], glm::vec3(0.1f, 0.0525f + 0.105f * i, 0.0f));
        ball.velocity = glm::vec3(1.2f, 0.0f, 2.1f);
        if (ball.type != SELF) {
            std::random_device dev;
            std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);
            ball.position = glm::vec3(distrib(dev) * 1.5f, 0.0525f, distrib(dev) * 0.7f);
            ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        entities.push_back(ball);
    }
}

void Snooker::renderSkybox() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Snooker::renderTable() {
    billiardTable.render(program);
    for (Entity &entity : entities) {
        entity.render(program);
    }
}

void Snooker::renderHoles() {
    
}

void Snooker::renderBalls() {
    
}

void Snooker::renderCueStick() {
    
}

void Snooker::renderTestTriangle() { 
    program.use();
    glBindVertexArray(testTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Snooker::update() {
    double thisInstant = glfwGetTime();
    float deltaTime = (float) (thisInstant - lastInstant);
    lastInstant = thisInstant;
    for (int i = 0; i < entities.size(); i++) {
        Entity &entity = entities[i];
        entity.update(deltaTime, &entities, i);
    }
    
    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_R)) {
        std::random_device dev;
        std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);
        for (int i = 0; i < 16; i++) {
            Entity &ball = entities[i];
            if (ball.type != SELF) {
                ball.position = glm::vec3(distrib(dev) * 1.5f, 0.0525f, distrib(dev) * 0.7f);
                ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            } else {
                ball.velocity = glm::vec3(distrib(dev) * 10.0f, 0.0f, distrib(dev) * 10.0f);
            }
        }
    }
}

float t = 0.0f;

void Snooker::applyRegularCamera() {
    t += 0.001f;
    view = glm::lookAt(glm::vec3(-3.0f * cosf(t), 2.0f, -3.0f * sinf(t)), entities[0].position, glm::vec3(0.0f, 1.0f, 0.0f));
    perspective = glm::perspective(glm::radians(45.0f),
                                   windowWrapper->getFrameBufferSize().x / windowWrapper->getFrameBufferSize().y,
                                   0.01f,
                                   2000.0f);

    // === APPLY TO TEST SHADER === //
    program.applyVP(view, perspective);
}

void Snooker::loadBallModels() {
    ballModels.clear();
    for (int i = 0; i < 16; i++) {
        Model ballModel = Model("Assets/Balls/Balls.obj", "Assets/Balls");
        ballModel.load(i, &textureStore);
        ballModel.modelMatrix = glm::translate(ballModel.modelMatrix, glm::vec3(0.0f, 0.0525f + 0.105f * i, 0.0f));
        ballModels.push_back(ballModel);
    }
}


