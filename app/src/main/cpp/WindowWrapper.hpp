//
//  WindowWrapper.hpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#ifndef WindowWrapper_hpp
#define WindowWrapper_hpp

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Ext/glm/glm.hpp"


class WindowWrapper {
public:
    void init(int w, int h, const char *title);
    
    bool shouldClose();
    void pollEvents();
    void swapBuffers();

    glm::vec2 getWindowSize();
    glm::vec2 getFrameBufferSize();

    // Warning: Might not work!
    const void *getNativeWindow();

    // === CACHES === //
    glm::vec2 cachedWindowSize;
    glm::vec2 cachedFrameBufferSize;

private:
    void *nativeWindow;
};

#endif /* WindowWrapper_hpp */
