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


Entity::Entity(EntityType type, Model *model, glm::vec3 position) : type(type), model(model), position(position), velocity(0.0f), acceleration(0.0f), radius(0.0525f), rou(1.225f), Cd(0.1f), g(9.8f), coef(0.05f) {
    A = (4.0f / 3.0f * PI * powf(radius, 2.0f)) / 2.0f;
}

void Entity::update(float deltaTime) {
    if (position.x + radius > 2.0f || position.x - radius < -2.0f) {
        velocity.x = -velocity.x;
    }
    if (position.z + radius > 1.0f || position.z - radius < -1.0f) {
        velocity.z = -velocity.z;
    }
    glm::vec3 oldVelocity = velocity;
    position = position + velocity * deltaTime;
    velocity = velocity + acceleration * deltaTime;
    std::cout << "velocity is now " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
    if ((velocity.x == 0.0f && velocity.y == 0.0f && velocity.z == 0.0f) ||
        velocity.x * oldVelocity.x < 0 || velocity.y * oldVelocity.y < 0 ||
        velocity.z * oldVelocity.z < 0) {
        velocity = glm::vec3(0.0f);
        acceleration = glm::vec3(0.0f);
    } else {
        acceleration = g * coef * glm::normalize(-velocity);
        acceleration += -(1.0f / 2.0f * rou * Cd * A * -glm::pow(velocity, glm::vec3(2.0f))) / 0.1559f;
    }
    model->modelMatrix = glm::translate(glm::mat4(1.0f), position);
}

void Entity::render(StandardProgram &program) { 
    model->render(program);
}


