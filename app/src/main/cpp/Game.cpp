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
    this->renderState = MENU;
    fingerPressed = false;
    initImGui();
}

void Game::render() {
    Result result;

    switch (this->renderState) {
    case MENU:
        result = showMenu();
        switch (result) {
        case EXIT:
            std::exit(0);

        case START_GAME:
            this->renderState = SNOOKER;
            initSnookerRuntime();
            break;

        case START_OPTION:
            this->renderState = OPTIONS;
            break;

        default:
            break;
        }
        break;

    case SNOOKER:
        startGame();
        break;
    }
}

Result Game::showMenu() {
    windowWrapper->pollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Result result = NOT_DECIDED_YET;

    // === USER CONTENT === //
    ImGui_ImplOpenGL3_NewFrame();
    imGuiIO->MouseDown[0] = fingerPressed;
    imGuiIO->MousePos = ImVec2(fingerPos.x * windowWrapper->getWindowSize().x, fingerPos.y * windowWrapper->getWindowSize().y);
    ImGui::NewFrame();
    bool opened = true;
    ImGui::Begin("Snooker3D Game Menu", &opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowSize({ windowWrapper->getWindowSize().x, 300 });
    ImGui::SetWindowPos({ 0, 0 });
    ImGui::Text("Welcome to Snooker3D. It is still in\nvery very early alpha.");
    if (ImGui::CollapsingHeader("Start Game")) {
        result = START_GAME;
    }
    if (ImGui::CollapsingHeader("Options")) {
        result = START_OPTION;
    }
    if (ImGui::CollapsingHeader("Exit")) {
        result = EXIT;
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    windowWrapper->swapBuffers();
    return result;
}

void Game::showImGuiDemoWindow() {
    windowWrapper->pollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    imGuiIO->MouseDown[0] = fingerPressed;
    imGuiIO->MousePos = ImVec2(fingerPos.x * windowWrapper->getWindowSize().x, fingerPos.y * windowWrapper->getWindowSize().y);
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    windowWrapper->swapBuffers();
}

void Game::showWinner() {
    
}

void Game::startGame() {
    windowWrapper->pollEvents();
    snookerGame->update();
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    snookerGame->applyRegularCamera();
    snookerGame->renderSkybox();
    snookerGame->renderTable();
//        snooker.renderTable();
//        snooker.renderHoles();
//        snooker.renderBalls();
//        snooker.renderCueStick();

    windowWrapper->swapBuffers();
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
    this->mapKeys(); // There is no keys, but we can specify the touch mode
}

void Game::mapKeys() { 
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    assert(this->imGuiIO != nullptr);
    ImGuiIO &io = *this->imGuiIO;
    // DEPRECATED: THERE IS NO KEYS
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
}

WindowWrapper *Game::getWindowWrapper() {
    return this->windowWrapper;
}

ImGuiIO *Game::getImGuiIO() {
    return imGuiIO;
}

void Game::updateEvent(glm::vec2 fingerPos, bool fingerPressed) {
    this->fingerPos = fingerPos;
    this->fingerPressed = fingerPressed;
}

void Game::initSnookerRuntime() {
    snookerGame = new Snooker(windowWrapper);
    snookerGame->init();
}
