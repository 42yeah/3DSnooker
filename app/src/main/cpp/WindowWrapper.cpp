//
//  WindowWrapper.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "WindowWrapper.hpp"

void WindowWrapper::init(int w, int h, const char *title) {
    cachedFrameBufferSize = cachedWindowSize = { w, h };
    cachedWindowSize = cachedFrameBufferSize / 4.0f;
}

bool WindowWrapper::shouldClose() {
    return false;
}

void WindowWrapper::pollEvents() {
    //
}

void WindowWrapper::swapBuffers() {
    //
}

const void *WindowWrapper::getNativeWindow() {
    return this->nativeWindow;
}

glm::vec2 WindowWrapper::getWindowSize() {
    return cachedWindowSize;
}

glm::vec2 WindowWrapper::getFrameBufferSize() {
    return cachedFrameBufferSize;
}
