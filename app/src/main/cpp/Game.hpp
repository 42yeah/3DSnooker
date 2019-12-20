//
//  Game.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "WindowWrapper.hpp"
#include "Snooker.hpp"
#include "Ext/imgui/imgui.h"
#include "Resources.hpp"


enum Result {
    NOT_DECIDED_YET, START_GAME, START_OPTION, EXIT
};

enum RenderState {
    MENU, OPTIONS, SNOOKER
};

class Game {
public:
    Game() {}
    Game(Resources *loader);

    void init();
    void render();
    Result showMenu();
    void showWinner();
    void showImGuiDemoWindow();
    void startGame();
    void startOption();
    bool hasWinner();
    void processTurn();
    void updateEvent(glm::vec2 fingerPos, bool fingerPressed);

    WindowWrapper *getWindowWrapper();
    ImGuiIO *getImGuiIO();

private:
    void initImGui();
    void mapKeys();
    void initSnookerRuntime();

    WindowWrapper *windowWrapper;
    ImGuiIO *imGuiIO;
    glm::vec2 fingerPos;
    bool fingerPressed;
    RenderState renderState;
    Snooker *snookerGame;
    Resources *resourceLoader;
};

#endif /* Game_hpp */
