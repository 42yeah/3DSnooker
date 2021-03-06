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
#include <vector>
#include "Ext/glm/glm.hpp"
#include "StandardProgram.hpp"


// === HELPER CLASS === //
class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Texture {
public:
    Texture() {}
    Texture(std::string filename);
    
    void load(Resources *loader);
    bool isValid();
    
    GLuint glTexture;
    int w, h, channels;
    std::string filename;
    bool valid;
};

class TextureStore {
public:
    TextureStore(Resources *loader);

    Texture *load(std::string filename);

    std::vector<Texture *> textures;
    Resources *resourceLoader;
};

// === LA REAL THANG === //
class Model {
public:
    Model() {}
    Model(std::string path, std::string mtlBaseDir, std::string textureBaseDir);
    
    void load(Resources *loader, int index, TextureStore *store);
    void render(StandardProgram &program);

    std::string modelName;
    std::string modelPath;
    std::string modelMtlPath;
    std::string textureBaseDir;
    std::vector<Vertex> vertices;
    GLuint VBO;
    int numVertices;
    
    Texture *ambientTexture, *diffuseTexture, *specularTexture;
    
    glm::mat4 modelMatrix;
};

#endif /* Model_hpp */
