//
//  Snooker.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef Snooker_hpp
#define Snooker_hpp

#include "WindowWrapper.hpp"
#include "StandardProgram.hpp"


class Snooker {
public:
    Snooker(WindowWrapper *wrapper);
    
    void init();
    void update();
    void renderSkybox();
    void renderTable();
    void renderHoles();
    void renderBalls();
    void renderCueStick();
    
    // === TEST CODES === //
    void renderTestTriangle();

private:
    WindowWrapper *windowWrapper;
    
    // === TEST DATA === //
    GLuint testTriangleVAO, testTriangleVBO;
    StandardProgram testTriangleProgram;
};

#endif /* Snooker_hpp */
