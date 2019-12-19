//
//  Entity.cpp
//  Snooker3D
//
//  Created by apple on 2019/12/18.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Entity.hpp"
#include <cmath>

#define PI 3.14159265f


std::basic_ostream<char> &operator<<(std::basic_ostream<char> &ostream, const glm::vec3 &vec3) {
    std::ostream &ost = ostream << vec3.x << ", " << vec3.y << ", " << vec3.z;
    return ost;
}

Entity::Entity(EntityType type, Model *model, glm::vec3 position) : type(type), model(model), position(position), velocity(0.0f), acceleration(0.0f), radius(0.0525f), rou(1.225f), Cd(0.1f), g(9.8f), coef(0.05f) {
    A = (4.0f / 3.0f * PI * powf(radius, 2.0f)) / 2.0f;
    rollMatrix = glm::mat4(1.0f);
    holed = false;
}

void Entity::update(float deltaTime, std::vector<Entity> *entities, std::vector<glm::vec3> *holes, int index) {
    // If velocity is zero, there is no need to update anyway
    if (velocity == glm::vec3(0.0f)) {
        return;
    }
    if (position.x + radius > 2.0f) { position.x = 2.0f - radius; velocity.x = -velocity.x * 0.9f; }
    if (position.x - radius < -2.0f) { position.x = -2.0f + radius; velocity.x = -velocity.x * 0.9f; }
    if (position.z + radius > 1.0f) { position.z = 1.0f - radius; velocity.z = -velocity.z * 0.9f; }
    if (position.z - radius < -1.0f) { position.z = -1.0f + radius; velocity.z = -velocity.z * 0.9f; }
    for (int i = 0; i < holes->size() && !holed; i++) {
//        if (index == 0) {
//            std::cout << "Looking at " << holes->at(i) << " distance: " << glm::distance(position, holes->at(i)) << std::endl;
//        }
        if (glm::distance(position, holes->at(i)) <= radius * 3.5f) {
            holed = true;
            velocity = holes->at(i) - position;
            velocity.y = -0.5f;
            break;
        }
    }
    glm::mat3 rotation = {
        0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };
    for (int i = 0; i < entities->size() && !holed; i++) {
        if (i == index) { continue; }
        Entity &anotherBall = entities->at(i);
        if (anotherBall.holed) { continue; }
        float dist = glm::distance(position, anotherBall.position);
        if (dist <= radius * 2.0f) {
            // there is a collision
            glm::vec3 connection = position - anotherBall.position;
            glm::vec3 reflection = glm::normalize(rotation * connection);
            float averagedVelocity = (glm::length(velocity) + glm::length(anotherBall.velocity)) / 2.0f;
            glm::vec3 direction = glm::reflect(glm::normalize(velocity), reflection);
            velocity = averagedVelocity * direction;
            anotherBall.velocity = averagedVelocity * -direction;
            position = position + velocity * deltaTime;
            i = -1;
            continue;
        }
    }
    glm::vec3 oldVelocity = velocity;
    position = position + velocity * deltaTime;
    velocity = velocity + acceleration * deltaTime;
    rollMatrix = glm::rotate(rollMatrix, glm::radians(glm::length(velocity) * 360.0f) * deltaTime, glm::normalize(rotation * velocity));
//    std::cout << "velocity is now " << velocity << std::endl;
    if ((velocity.x == 0.0f && velocity.y == 0.0f && velocity.z == 0.0f) ||
        velocity.x * oldVelocity.x < 0 || velocity.y * oldVelocity.y < 0 ||
        velocity.z * oldVelocity.z < 0) {
        velocity = glm::vec3(0.0f);
        acceleration = glm::vec3(0.0f);
    } else {
        // Newtonian Mechanics
        acceleration = g * coef * glm::normalize(-velocity);
        acceleration += -(1.0f / 2.0f * rou * Cd * A * -glm::pow(velocity, glm::vec3(2.0f))) / 0.1559f;
    }
}

void Entity::render(StandardProgram &program) {
    model->modelMatrix = glm::translate(glm::mat4(1.0f), position) * rollMatrix;
    model->render(program);
}
