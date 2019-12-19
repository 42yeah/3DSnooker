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
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    GLFWwindow *window = glfwCreateWindow(w, h, title, nullptr, nullptr);
//    glfwMakeContextCurrent(window);
//    gladLoadGL();
//    this->nativeWindow = window;
//
//    for (int i = 0; i < 5; i++) {
//        this->cursorCache[i] = false;
//    }
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

glm::vec2 WindowWrapper::getCursorPosition() { 
    double x, y;
//    glfwGetCursorPos(nativeWindow, &x, &y);
    return glm::vec2((float) x, (float) y);
}

bool WindowWrapper::getCursorState(int i) { 
//    return glfwGetMouseButton(nativeWindow, i);
    return true;
}


