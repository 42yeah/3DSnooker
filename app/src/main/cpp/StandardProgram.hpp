//
//  StandardProgram.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/23.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef StandardProgram_hpp
#define StandardProgram_hpp

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <iostream>
#include "Ext/glm/glm.hpp"
#include "Ext/glm/gtc/matrix_transform.hpp"
#include "Ext/glm/gtc/type_ptr.hpp"
#include "Resources.hpp"

class Texture;

/**
 Explanation about why it being "standard":
 Standard only applies to all Snooker3D shaders, which means it has all uniforms we need,
 And the data are at the right place.
 */
class StandardProgram {
public:
    StandardProgram(Resources *loader);

    void link(std::string vertexShaderPath, std::string fragmentShaderPath);
    GLuint compile(GLuint shaderType, std::string shaderPath);
    void use();
    void applyM(glm::mat4 model);
    void applyVP(glm::mat4 view, glm::mat4 perspec);
    void applyTexture(Texture *ambient, Texture *diffuse, Texture *specular);
    void configVertexPointers();

private:
    GLuint program;
    Resources *resourceLoader;
    
    // === LOCATIONS === //
    GLint modelLoc, viewLoc, perspectiveLoc;
    GLint texturizeLoc, ambientLoc, diffuseLoc, specularLoc;
    GLuint aPosLoc, aNormalLoc, aTexCoordLoc;
};

#endif /* StandardProgram_hpp */
