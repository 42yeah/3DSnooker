#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "files.h"
#include "memory.h"
#include "gl_helpers.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


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
Memory memory;

// === EVENT HANDLER === //
extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_Engine_motionEvent(JNIEnv *env, jobject instance, jint action,
                                                   jfloat x, jfloat y) {
    // TODO
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
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    LOG("Version %s", glGetString(GL_VERSION));
    memory.poolProgram = linkProgramByPath(
            "shaders/generic.vertex.glsl",
            "shaders/pool.fragment.glsl",
            resources
    );
    memory.ballProgram = linkProgramByPath(
            "shaders/generic.vertex.glsl",
            "shaders/balls.fragment.glsl",
            resources
    );

    // === GL CONFIG === //
    glEnable(GL_DEPTH_TEST);

    // === MODELS === //
    memory.pool = Model(resources, "models/Pool.model");
    memory.pool.setRenderWireframe(true);
    memory.ball = Model(resources, "models/Ball.model");

    // === TESTS === //
    memory.testModel = Model(resources, "models/TestCube.model");

}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_surfaceChanged(JNIEnv *env, jobject instance, jint w,
                                                              jint h) {
    memory.aspectRatio = (float) w / h;
    memory.perspectiveMat = glm::perspective(45.0f, memory.aspectRatio, 0.1f, 100.0f);
    memory.viewMat = glm::lookAt(
            glm::vec3(3.0f, 0.7f, 2.5f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_render(JNIEnv *env, jobject instance) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // === TESTS === //
    // memory.testModel.render(memory.prog, memory);

    memory.poolProgram.use();
    configureCamera(memory.poolProgram, memory.viewMat, memory.perspectiveMat);
    memory.pool.render(memory.poolProgram, memory);

    memory.ballProgram.use();
    configureCamera(memory.ballProgram, memory.viewMat, memory.perspectiveMat);
    memory.ball.render(memory.ballProgram, memory);
}
