//
//  Entity.hpp
//  Snooker3D
//
//  Created by apple on 2019/12/18.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include "../Ext/glm/glm.hpp"
#include "../Ext/glm/gtc/matrix_transform.hpp"
#include "../Ext/glm/gtc/type_ptr.hpp"
#include "Model.hpp"
#include "StandardProgram.hpp"


enum EntityType {
    FRIEND, FOE, SELF, BLACK
};

/**
 Well, as there are only balls in this game,
 that means 'entity' roughly means 'da ball'. Whatever!
 As we are going to use an insanely simple physics model, we are just gonna count friction.
 */
class Entity {
public:
    Entity() {}
    Entity(EntityType type, Model *model, glm::vec3 position);
    
    void update(float deltaTime, std::vector<Entity> *entities, std::vector<glm::vec3> *holes, int index);
    void render(StandardProgram &program);
    
    float radius;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    Model *model;
    
    // === LOGIC === //
    EntityType type;
    glm::mat4 rollMatrix;
    bool holed;
    
    // === PHYSICS === //
    float rou; // ρ: The air density
    float Cd; // Cd: The resistant constant. Ball is 0.1.
    float A; // A: The area of air resistance. Half the ball.
    float g; // g: Gee force.
    float coef; // Friction.
};

#endif /* Entity_hpp */
