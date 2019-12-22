//
// Created by apple on 2019-12-21.
//

#include "SnookerController.hpp"
#include "Snooker.hpp"
#include "Ext/glm/glm.hpp"
#include "Macros.hpp"
#include <cmath>


SnookerController::SnookerController(Snooker *snookerGame, EntityType ballType) : instance(snookerGame), ballType(ballType) {
}

PlayerSnookerController::PlayerSnookerController(Snooker *snookerGame, EntityType ballType) : SnookerController(snookerGame, ballType), previousFingerDown(false) {

}

void PlayerSnookerController::notifyTurnArrived() {
    SnookerController::notifyTurnArrived();
    processingTurn = true;
}

void PlayerSnookerController::process() {
    glm::vec2 pos = instance->fingerPos;
    if (instance->fingerDown && !previousFingerDown) {
        fingerPosWhenDown = pos;
    }
    if (previousFingerDown) {
        float deltaX = pos.x - fingerPosWhenDown.x;
        float deltaY = pos.y - fingerPosWhenDown.y;
        float deltaDeg = deltaX * (PI / 2.0f);
        if (!instance->fingerDown) {
            instance->cachedRotation = instance->cachedRotation + deltaDeg;
            instance->rotation = instance->cachedRotation;
        } else {
            instance->rotation = instance->cachedRotation + deltaDeg;
        }
        if (deltaY >= 0.1f && !instance->ballsMoving() && processingTurn) {
            instance->force = fmin(deltaY - 0.1f, 0.3f) / 0.3f * 11.0f;
            if (!instance->fingerDown) {
                glm::vec3 ray = glm::normalize(instance->entities[0].position - instance->camPos);
                ray.y = 0.0f;
                instance->entities[0].velocity = glm::normalize(ray) * instance->force;
                instance->force = 0.0f;
                processingTurn = false;
            }
        } else {
            instance->force = 0.0f;
        }
        if (deltaY <= -0.1f && !instance->ballsMoving()) {
            instance->aim = fmin(fabs(deltaY) - 0.1f, 0.3f) / 0.3f * 0.8f;
        }
    } else {
        instance->aim *= 0.5f;
    }
    previousFingerDown = instance->fingerDown;
}

NPSnookerController::NPSnookerController(Snooker *snookerGame, EntityType ballType)
        : SnookerController(snookerGame, ballType) , processingTurn(false) {
}

void NPSnookerController::notifyTurnArrived() {
    SnookerController::notifyTurnArrived();
    for (int i = 0; i < instance->entities.size(); i++) {
        Entity &entity = instance->entities[i];
        if ((entity.type == this->ballType || entity.type == BLACK) && !entity.holed) {
            // Let's aim at this!
            glm::vec3 connection = glm::normalize(entity.position - instance->entities[0].position);
            glm::vec3 subX = glm::vec3(connection.x, 0.0f, 0.0f);
            expectedRotation = asinf(connection.z);
            if (connection.x < 0.0f) {
                expectedRotation = PI - expectedRotation;
            }
            expectedForce = fabs(instance->distrib->operator()(instance->dev)) * 8.0f + 3.0f;
            break;
        }
    }
    processingTurn = true;
    instance->force = 0.0f;
}

void NPSnookerController::process() {
    if (instance->ballsMoving() || !processingTurn) { return; }
    float deltaAngle = expectedRotation - instance->rotation;
    instance->rotation += deltaAngle * instance->deltaTime;
    instance->cachedRotation = instance->rotation;
    if (fabs(deltaAngle) <= 0.01f || instance->force > 0.0f) {
        float deltaForce = expectedForce - instance->force;
        instance->force += deltaForce * instance->deltaTime;
        if (fabs(deltaForce) <= 0.1f) {
            glm::vec3 ray = glm::normalize(instance->entities[0].position - instance->camPos);
            ray.y = 0.0f;
            instance->entities[0].velocity = glm::normalize(ray) * instance->force;
            instance->force = 0.0f;
            processingTurn = false;
        }
    }
}
