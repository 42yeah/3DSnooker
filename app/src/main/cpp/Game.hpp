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
#include "Ext/imgui/imgui.h"


enum Result {
    NOT_DECIDED_YET, START_GAME, START_OPTION, EXIT
};

class Game {
public:
    Game() {}

    void init();
    Result showMenu();
    void showWinner();
    void showImGuiDemoWindow();
    void startGame();
    void startOption();
    bool hasWinner();
    void processTurn();

    WindowWrapper *getWindowWrapper();
    ImGuiIO *getImGuiIO();
    glm::vec2 fingerPos;
    bool fingerPressed;

private:
    WindowWrapper *windowWrapper;
    ImGuiIO *imGuiIO;
    
    void initImGui();
    void mapKeys();
    void updateCursor();
};

#endif /* Game_hpp */
