//
//  Model.hpp
//  Snooker3D
//
//  Created by apple on 2019/12/9.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include <iostream>
#include "../Ext/glm/glm.hpp"
#include "StandardProgram.hpp"


class Model {
public:
    Model() {}
    Model(std::string path, std::string mtlBaseDir);
    
    void load();
    void render(StandardProgram &program);

    std::string modelName;
    std::string modelPath;
    std::string modelMtlBaseDir;
    GLuint VAO;
    int numVertices;
};

// === HELPER CLASS === //
class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

#endif /* Model_hpp */
