//
//  Snooker.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Snooker.hpp"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <chrono>
#include "Ext/glm/glm.hpp"
#include "Ext/glm/gtc/matrix_transform.hpp"
#include "Ext/glm/gtc/type_ptr.hpp"
#include "Model.hpp"


Snooker::Snooker(WindowWrapper *wrapper, Resources *loader) : windowWrapper(wrapper), resourceLoader(loader), program(loader), textureStore(loader) {
    
}

void Snooker::init() {
    globalRotation = glm::mat4(1.0f);

    // === TEST DATA === //
    program.link("shaders/standard.vertex.glsl", "shaders/standard.fragment.glsl");

    glGenBuffers(1, &testTriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, testTriangleVBO);
    float testTriangleData[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(testTriangleData), testTriangleData, GL_STATIC_DRAW);
    program.configVertexPointers();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glViewport(0, 0, windowWrapper->getFrameBufferSize().x, windowWrapper->getFrameBufferSize().y);
    glEnable(GL_DEPTH_TEST);

    billiardTable = Model("models/PoolTable/PoolTable.obj", "models/PoolTable/PoolTable.mtl", "models/PoolTable");
    billiardTable.load(resourceLoader, -1, &textureStore);
    loadBallModels();
    cue = Model("models/Cue/PoolCue.obj", "models/Cue/PoolCue.mtl", "models/Cue");
    cue.load(resourceLoader, -1, &textureStore);

    // === TIME === //
    lastInstant = epoch();
    cachedRotation = rotation = 0.0f;
    previousFingerState = false;
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
        ball.velocity = glm::vec3(1.2f, 0.0f, 2.1f);
        if (ball.type != SELF) {
//            ball.position = glm::vec3(distrib(dev) * 1.5f, 0.0525f, distrib(dev) * 0.7f);
            ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        }
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
}

void Snooker::renderBalls() {
    for (Entity &entity : entities) {
        entity.render(program);
    }
}

void Snooker::renderCueStick() {
    glDisable(GL_DEPTH_TEST);
    cue.render(program);
    glEnable(GL_DEPTH_TEST);
}

void Snooker::renderTestTriangle() {
    glBindBuffer(GL_ARRAY_BUFFER, testTriangleVBO);
    program.use();
    program.applyM(glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Snooker::update() {
    double thisInstant = epoch();
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
    
//    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_SPACE)) {
//        force += deltaTime * 10.0f;
//        if (force >= 11.0f) { force = 11.0f; }
//    } else if (force != 0.0f && !glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_SPACE)) {
//        ray.y = 0.0f;
//        entities[0].velocity = glm::normalize(ray) * force;
//        force = 0.0f;
//    }
//    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_A)) {
//        rotation -= 1.57f * deltaTime;
//    }
//    if (glfwGetKey((GLFWwindow *) windowWrapper->getNativeWindow(), GLFW_KEY_D)) {
//        rotation += 1.57f * deltaTime;
//    }
}

void Snooker::applyRegularCamera() {
    camPos = glm::vec3(entities[0].position.x - 3.2f * cosf(rotation), 0.8f, entities[0].position.z - 3.2f * sinf(rotation));
//    view = glm::lookAt(glm::vec3(3.0f * cosf(t), 1.5f, sinf(t) * 1.0f), entities[0].position, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::lookAt(camPos, entities[0].position, glm::vec3(0.0f, 1.0f, 0.0f));
    perspective = glm::perspective(glm::radians(45.0f),
                                   windowWrapper->getFrameBufferSize().x / windowWrapper->getFrameBufferSize().y,
                                   0.01f,
                                   200.0f);

    // === APPLY TO TEST SHADER === //
    program.applyVP(view, perspective);
}

void Snooker::loadBallModels() {
    ballModels.clear();
    for (int i = 0; i < 16; i++) {
        Model ballModel = Model("models/Balls/Balls.obj", "models/Balls/Balls.mtl", "models/Balls");
        ballModel.load(resourceLoader, i, &textureStore);
        ballModel.modelMatrix = glm::translate(ballModel.modelMatrix, glm::vec3(0.0f, 0.0525f + 0.105f * i, 0.0f));
        ballModels.push_back(ballModel);
    }
}

double Snooker::epoch() {
    long long int sinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return (double) sinceEpoch / 1000.0;
}

void Snooker::handleEvent(bool down, glm::vec2 pos) {
    if (down && !previousFingerState) {
        fingerPosWhenDown = pos;
    }
    if (previousFingerState) {
        float deltaX = pos.x - fingerPosWhenDown.x;
        float deltaY = pos.y - fingerPosWhenDown.y;
        float deltaDeg = deltaX * (PI / 2.0f);
        if (!down) {
            cachedRotation = cachedRotation + deltaDeg;
            rotation = cachedRotation;
        } else {
            rotation = cachedRotation + deltaDeg;
        }
        if (deltaY >= 0.1f) {
            force = fmin(deltaY - 0.1f, 0.3f) / 0.3f * 11.0f;
            if (!down) {
                glm::vec3 ray = glm::normalize(entities[0].position - camPos);
                ray.y = 0.0f;
                entities[0].velocity = glm::normalize(ray) * force;
                force = 0.0f;
            }
        } else {
            force = 0.0f;
        }
    }
    previousFingerState = down;
}
