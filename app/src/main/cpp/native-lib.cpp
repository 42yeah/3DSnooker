#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <chrono>
#include <cmath>
#include "Resources.hpp"
#include "Game.hpp"
#include "Macros.hpp"


// === TESTS === //
extern "C" JNIEXPORT jstring JNICALL
Java_aiofwa_org_a3dsnooker_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "JNI works. Loading from Snooker Engine...";

    return env->NewStringUTF(hello.c_str());
}

// === SIMPLETONS === //
Resources *resources;
Game *game;

// === EVENT HANDLER === //
extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_motionEvent(JNIEnv *env, jobject instance, jint action,
                                                   jfloat x, jfloat y) {
    game->getImGuiIO()->MousePos = { (float) x * game->getWindowWrapper()->getWindowSize().x,
                                     (float) y * game->getWindowWrapper()->getWindowSize().y };
    switch (action) {
        case 0:
            // === PRESSING DOWN ===
            game->updateEvent(glm::vec2(x, y), true);
            break;

        case 2:
            // === MOVING ===
            game->updateEvent(glm::vec2(x, y), true);
            break;

        case 1:
            // === RELEASING ===
            game->updateEvent(glm::vec2(x, y), false);
            break;
    }
}

// === RENDER === //
extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_setAssetManager(JNIEnv *env, jobject instance,
                                                               jobject manager) {
    resources = new Resources(AAssetManager_fromJava(env, manager));
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_surfaceCreated(JNIEnv *env, jobject instance, jint initialW, jint initialH) {
    game = new Game(resources);
    game->init(initialW, initialH);
    LOG("ImGui Initialized\n");
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_surfaceChanged(JNIEnv *env, jobject instance, jint w,
                                                              jint h) {
    game->getWindowWrapper()->cachedFrameBufferSize = { (float) w, (float) h };
    game->getWindowWrapper()->cachedWindowSize = game->getWindowWrapper()->cachedFrameBufferSize / 4.0f;
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_render(JNIEnv *env, jobject instance) {
    game->render();
}

extern "C"
JNIEXPORT jint JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_getSnookerWinner(JNIEnv *env, jobject thiz) {
    if (!game) {
        return 0;
    }
    return game->showWinner();
}

extern "C"
JNIEXPORT jint JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_getSnookerControllingController(JNIEnv *env, jobject thiz) {
    if (!game) {
        return -1;
    }
    return game->getControllingController();
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_backToMainMenu(JNIEnv *env, jobject thiz) {
    game->backToMainMenu();
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_shutdown(JNIEnv *env, jobject thiz) {
    LOG("Shutting down game");
    game->shutdown();
}
