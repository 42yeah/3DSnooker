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
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Ext/imgui/imgui.h"
#include "Ext/imgui/implements/imgui_impl_opengl3.h"
#include "Ext/glm/glm.hpp"
#include "Snooker.hpp"


void Game::init() {
    windowWrapper = new WindowWrapper();
    windowWrapper->init(1080, 1800, "Snooker3D");
    fingerPos = glm::vec2(0.0f, 0.0f);
    fingerPressed = false;
    initImGui();
}

Result Game::showMenu() {
    windowWrapper->pollEvents();
    updateCursor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    // === USER CONTENT === //
    ImGui_ImplOpenGL3_NewFrame();
    imGuiIO->MouseDown[0] = fingerPressed;
    if (fingerPressed) { fingerPressed = false; }
    imGuiIO->MousePos = ImVec2(fingerPos.x * windowWrapper->getWindowSize().x, fingerPos.y * windowWrapper->getWindowSize().y);
    ImGui::NewFrame();
    bool opened = true;
//    LOG("EVENT: %f %f, Pressed: %d", imGuiIO->MousePos.x, imGuiIO->MousePos.y, imGuiIO->MouseDown[0]);
    ImGui::Begin("Snooker3D Game Menu", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowSize({ windowWrapper->getWindowSize().x, 300 });
    ImGui::SetWindowPos({ 0, 0 });
    ImGui::Text("Welcome to Snooker3D. It is still in\nvery very early alpha.");
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
    return NOT_DECIDED_YET;
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

        snooker.applyRegularCamera();
        snooker.renderSkybox();
        snooker.renderTable();
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

    ImGui_ImplOpenGL3_Init("#version 100");

    glm::vec2 winSize = windowWrapper->getWindowSize();
    glm::vec2 frameBufferSize = windowWrapper->getFrameBufferSize();
    LOG("Viewport: %f %f", winSize.x, winSize.y);
    glViewport(0, 0, winSize.x, winSize.y);

    io.DisplaySize.x = windowWrapper->getWindowSize().x;
    io.DisplaySize.y = windowWrapper->getWindowSize().y;
    io.DisplayFramebufferScale = ImVec2(frameBufferSize.x / winSize.x,
                                        frameBufferSize.y / winSize.y);
    this->imGuiIO = &io;
//    this->mapKeys(); There is no keys
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
    assert(this->imGuiIO != nullptr);
    ImGuiIO &io = *this->imGuiIO;
    // DEPRECATED: THERE IS NO KEYS
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

WindowWrapper *Game::getWindowWrapper() {
    return this->windowWrapper;
}

ImGuiIO *Game::getImGuiIO() {
    return imGuiIO;
}

