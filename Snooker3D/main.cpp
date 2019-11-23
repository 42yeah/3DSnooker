//
//  main.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

// === DOCUMENTATION === //
/**
    This is the snooker game, it is used to be ported to android later, however
    as working on computer is *quite* easier than working on phone, thus the movement
    will be applied later when it enters a more mature stage.
 */

#include "Game.hpp"


// === MAIN APPLICATION ENTRY === //
int finalMain(int argc, char *argv[]) {
    Game game = Game();
    game.init();
    switch (game.showMenu()) {
        case START_GAME:
            game.startGame();
            while (!game.hasWinner()) {
                game.processTurn();
            }
            game.showWinner();
            break;
            
        case START_OPTION:
            game.startOption();
            break;
            
        case EXIT:
            exit(0);
            break;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    Game game = Game();
    game.init();
    game.showMenu();
    game.startGame();
    return 0;
}
