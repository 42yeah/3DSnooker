//
//  Snooker.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef Snooker_hpp
#define Snooker_hpp

#include <vector>
#include <random>
#include "WindowWrapper.hpp"
#include "StandardProgram.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "Resources.hpp"
#include "SnookerController.hpp"


enum WinningResult {
    COMPETITING, HUMAN, STUPID_NPC, DRAW
};

class Snooker {
public:
    Snooker(WindowWrapper *wrapper, Resources *loader);

    void reset();
    void init();
    void update();
    void renderSkybox();
    void renderTable();
    void renderBalls();
    void renderCueStick();
    void applyRegularCamera();
    void handleEvent(bool down, glm::vec2 pos);
    void processTurn();
    WinningResult getWinner();
    int getControllingController();
    
    // === HELPERS === //
    void loadBallModels();
    double epoch();
    bool ballsMoving();
    
    // === TEST CODES === //
    void renderTestTriangle();

private:
    float ballsMovingTime;

    WindowWrapper *windowWrapper;
    
    // === CAMERA === //
    glm::mat4 view, perspective;
    
    // === SERIOUS DATA === //
    Model billiardTable;
    std::vector<Model> ballModels;
    Model cue;

public:
    std::vector<Entity> entities;
    std::vector<glm::vec3> holes;
    StandardProgram program;
    TextureStore textureStore;
    double lastInstant;
    float deltaTime;
    glm::vec3 camPos;
    float cachedRotation;
    float rotation;
    float force;
    float aim;
    std::vector<SnookerController *> controllers;
    int controllingController;
    bool fingerDown;
    glm::vec2 fingerPos;
    Entity *deathCamTarget;

    // === RANDOMNESS === //
    std::random_device dev;
    std::uniform_real_distribution<float> *distrib;

private:
    Resources *resourceLoader;

    // === TEST DATA === //
    GLuint testTriangleVBO;
};

#endif /* Snooker_hpp */
