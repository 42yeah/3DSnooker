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


class Model {
public:
    Model() {}
    Model(std::string path, std::string mtlBaseDir);
    
    void load();

    std::string modelName;
    std::string modelPath;
    std::string modelMtlBaseDir;
};

#endif /* Model_hpp */
