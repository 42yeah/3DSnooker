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
#include <android/log.h>

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "Snooker3D " ":" S__LINE__, __VA_ARGS__)


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

    // === CACHES === //
    glm::vec2 cachedWindowSize;
    glm::vec2 cachedFrameBufferSize;

private:
    void *nativeWindow;
    bool cursorCache[5];
};

#endif /* WindowWrapper_hpp */
