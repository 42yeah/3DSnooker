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
#include "WindowWrapper.hpp"
#include "StandardProgram.hpp"
#include "Model.hpp"
#include "Entity.hpp"


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
    
    // === HELPERS === //
    void loadBallModels();
    
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
    std::vector<Model> ballModels;
    std::vector<Entity> entities;
    StandardProgram program;
    TextureStore textureStore;
    double lastInstant;

    // === TEST DATA === //
    GLuint testTriangleVAO, testTriangleVBO;
    
    GLuint testCubeVAO;
};

#endif /* Snooker_hpp */
