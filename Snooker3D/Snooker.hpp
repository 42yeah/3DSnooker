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
#include "Model.hpp"


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
    void applyRegularCamera();
    
    // === TEST CODES === //
    void renderTestTriangle();
    void renderTestCube();

private:
    WindowWrapper *windowWrapper;
    glm::mat4 globalRotation;
    
    GLuint poolTableVAO;
    
    // === CAMERA === //
    glm::mat4 view, perspective;
    
    // === SERIOUS DATA === //
    Model billiardTable;

    // === TEST DATA === //
    GLuint testTriangleVAO, testTriangleVBO;
    StandardProgram testTriangleProgram;
    
    GLuint testCubeVAO;
};

#endif /* Snooker_hpp */
