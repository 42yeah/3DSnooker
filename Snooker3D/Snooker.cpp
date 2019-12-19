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

void Snooker::init() {
    globalRotation = glm::mat4(1.0f);
    
    // === POOL TABLE === //

    
    // === TEST DATA === //
    program.link("Assets/standard.vertex.glsl",
                             "Assets/standard.fragment.glsl");
    
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
    cue = Model("Assets/Cue/PoolCue.obj", "Assets/Cue");
    cue.load(-1, &textureStore);
    
    // === TIME === //
    lastInstant = glfwGetTime();
    rotation = 0.0f;
    distrib = new std::uniform_real_distribution<float>(-1.0f, 1.0f);
    force = 0.0f;
    
    // === ENTITIES === //
    int indices[] = {
        0,
        1, 2, 3, 11, 12, 13, 15,
        4, 5, 6, 7,   8,  9, 14,
        10
    };

    glm::vec3 currentPos(-1.2f, 0.0525f, 0.0f);
    int numRows = 1;
    int currentRow = 1;
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
        Entity ball(entityType, &ballModels[indices[i]], currentPos);
        if (i == 0) {
            currentPos = glm::vec3(0.3f, 0.0525f, 0.0f);
        }
        if (i >= 1) {
            currentPos.z += 0.0526f * 2.0f;
            std::cout << "Row #" << numRows << std::endl;
            if (currentRow > numRows - 1) {
                currentRow = 0;
                numRows++;
                currentPos.x += 0.0526f * 2.0f;
                currentPos.z = -0.0526f * 2.0f * numRows / 2.0f + 0.0526f;
            }
            currentRow++;
        }
//        ball.velocity = glm::vec3(1.2f, 0.0f, 2.1f);
//        if (ball.type != SELF) {
//            ball.position = glm::vec3(distrib(dev) * 1.5f, 0.0525f, distrib(dev) * 0.7f);
//            ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
//        }
        entities.push_back(ball);
    }
    
    // === HOLES === //
    holes.push_back(glm::vec3(-2.0f, 0.0f, -1.0f));
    holes.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    holes.push_back(glm::vec3(2.0f, 0.0f, -1.0f));
    holes.push_back(glm::vec3(-2.0f, 0.0f, 1.0f));
    holes.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    holes.push_back(glm::vec3(2.0f, 0.0f, 1.0f));
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
    glDisable(GL_DEPTH_TEST);
    cue.render(program);
    glEnable(GL_DEPTH_TEST);
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
        entity.update(deltaTime, &entities, &holes, i);
    }

    camPos.y -= 1.0f;
    glm::vec3 ray = glm::normalize(entities[0].position - camPos);
    glm::vec3 stickPos = entities[0].position - ray * (0.72f + (force / 11.0f) * 0.5f);
    float tremble = distrib->operator()(dev) * (force / 11.0f) * 0.01f;
    stickPos += tremble + tremble;
    glm::vec3 sighting = glm::cross(glm::normalize(ray), glm::vec3(0.0f, 1.0f, 0.0f));
    float cosine = acosf(-ray.y);
    cue.modelMatrix = glm::rotate(
                                  glm::translate(glm::mat4(1.0f), stickPos),
                                  cosine + glm::radians(180.0f),
                                  sighting
                                  );
    
    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_SPACE)) {
        force += deltaTime * 10.0f;
        if (force >= 11.0f) { force = 11.0f; }
    } else if (force != 0.0f && !glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_SPACE)) {
        ray.y = 0.0f;
        entities[0].velocity = glm::normalize(ray) * force;
        force = 0.0f;
    }
    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_A)) {
        rotation -= 1.57f * deltaTime;
    }
    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_D)) {
        rotation += 1.57f * deltaTime;
    }
    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_R)) {
        std::random_device dev;
        std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);
        for (int i = 0; i < 1; i++) {
            Entity &ball = entities[i];
            ball.holed = false;
            ball.position.y = 0.0525f;
            ball.velocity.y = 0.0f;
//            ball.position = glm::vec3(distrib(dev) * 1.5f, 0.0525f, distrib(dev) * 0.7f);
            ball.position = glm::vec3(-1.2f, 0.0525f, 0.0f);
            if (ball.type != SELF) {
                ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            } else {
//                ball.velocity = glm::vec3(distrib(dev) * 10.0f, 0.0f, distrib(dev) * 10.0f);
                ball.velocity = glm::vec3(10.0f, 0.0f, 0.7f);
            }
        }
    }
}

void Snooker::applyRegularCamera() {
    camPos = glm::vec3(entities[0].position.x - 1.2f * cosf(rotation), 0.8f, entities[0].position.z - 1.2f * sinf(rotation));
//    view = glm::lookAt(glm::vec3(3.0f * cosf(t), 1.5f, sinf(t) * 1.0f), entities[0].position, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::lookAt(camPos, entities[0].position, glm::vec3(0.0f, 1.0f, 0.0f));
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


