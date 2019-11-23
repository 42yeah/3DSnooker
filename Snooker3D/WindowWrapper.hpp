//
//  WindowWrapper.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef WindowWrapper_hpp
#define WindowWrapper_hpp

#include "../Ext/glad/glad.h"
#include "../Ext/glm/glm.hpp"
#include <GLFW/glfw3.h>


class WindowWrapper {
public:
    void init(int w, int h, const char *title);
    
    bool shouldClose();
    void pollEvents();
    void swapBuffers();

    glm::vec2 getWindowSize();
    glm::vec2 getFrameBufferSize();
    glm::vec2 getCursorPosition();
    bool getCursorState(int i);

    // Warning: Might not work!
    const void *getNativeWindow();
    
private:
    GLFWwindow *nativeWindow;
    glm::vec2 cachedWindowSize;
    glm::vec2 cachedFrameBufferSize;
    bool cursorCache[5];
};

#endif /* WindowWrapper_hpp */
