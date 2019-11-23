//
//  Game.cpp
//  Snooker3D
//
//  Created by apple on 2019/11/22.
//  Copyright © 2019 aiofwa. All rights reserved.
//

#include "Game.hpp"
#include <iostream>
#include <cassert>
#include "../Ext/glad/glad.h"
#include "../Ext/imgui/imgui.h"
#include "../Ext/imgui/implements/imgui_impl_opengl3.h"
#include "../Ext/glm/glm.hpp"
#include "Snooker.hpp"


void Game::init() {
    windowWrapper = new WindowWrapper();
    windowWrapper->init(800, 600, "Snooker3D");
    initImGui();
}

Result Game::showMenu() {
    while (!windowWrapper->shouldClose()) {
        windowWrapper->pollEvents();
        updateCursor();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // === USER CONTENT === //
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        bool opened = true;
        ImGui::Begin("Snooker3D Game Menu", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowSize({ windowWrapper->getWindowSize().x, 300 });
        ImGui::SetWindowPos({ 0, windowWrapper->getWindowSize().y / 2.0f - 150 });
        ImGui::Text("Welcome to Snooker3D. It is still in very very early alpha.");
        if (ImGui::CollapsingHeader("Start Game")) {
            return START_GAME;
        }
        if (ImGui::CollapsingHeader("Options")) {
            return START_OPTION;
        }
        if (ImGui::CollapsingHeader("Exit")) {
            return EXIT;
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        windowWrapper->swapBuffers();
    }
    return EXIT;
}

void Game::showImGuiDemoWindow() {
    while (!windowWrapper->shouldClose()) {
        windowWrapper->pollEvents();
        updateCursor();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        windowWrapper->swapBuffers();
    }
}

void Game::showWinner() {
    
}

void Game::startGame() { 
    Snooker snooker(windowWrapper);
    snooker.init();
    while (!windowWrapper->shouldClose()) {
        windowWrapper->pollEvents();
        updateCursor();
        snooker.update();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        snooker.renderSkybox();
        snooker.renderTestTriangle();
        snooker.renderTestCube();
        snooker.renderTestSuzzane();
//        snooker.renderTable();
//        snooker.renderHoles();
//        snooker.renderBalls();
//        snooker.renderCueStick();

        windowWrapper->swapBuffers();
    }
}

void Game::startOption() {
    
}

bool Game::hasWinner() {
    return false;
}

void Game::processTurn() {
    
}

void Game::initImGui() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    glm::vec2 winSize = windowWrapper->getWindowSize();
    glm::vec2 frameBufferSize = windowWrapper->getFrameBufferSize();
    glViewport(0, 0, winSize.x, winSize.y);

    io.DisplaySize.x = windowWrapper->getWindowSize().x;
    io.DisplaySize.y = windowWrapper->getWindowSize().y;
    io.DisplayFramebufferScale = ImVec2(frameBufferSize.x / winSize.x,
                                        frameBufferSize.y / winSize.y);
    this->imGuiIO = &io;
    this->mapKeys();
}

void Game::updateCursor() {
    assert(this->imGuiIO != nullptr);
    ImGuiIO &io = *this->imGuiIO;
    glm::vec2 cursorPos = windowWrapper->getCursorPosition();
    io.MousePos = { cursorPos.x, cursorPos.y };
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        io.MouseDown[i] = windowWrapper->getCursorState(i);
    }
}

void Game::mapKeys() { 
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    // Stole directly from imgui_impl_glfw.cpp
    assert(this->imGuiIO != nullptr);
    ImGuiIO &io = *this->imGuiIO;
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

