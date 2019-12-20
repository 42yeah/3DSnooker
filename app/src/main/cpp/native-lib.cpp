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


// === TESTS === //
extern "C" JNIEXPORT jstring JNICALL
Java_aiofwa_org_a3dsnooker_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

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
    LOG("Pressin' %d", action);
    switch (action) {
        case 0:
            // === PRESSING DOWN ===
            game->updateEvent(glm::vec2(x, y), true);
            break;

        case 2:
            // === MOVING ===
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
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_surfaceCreated(JNIEnv *env, jobject instance) {
    game = new Game();
    game->init();
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
