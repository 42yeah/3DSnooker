#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <chrono>
#include <cmath>
#include "files.h"
#include "memory.h"
#include "gl_helpers.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "nuklear.h"


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
    switch (action) {
        case 0:
            memory.originX = x;
            memory.originY = y;
            memory.pitchBase = memory.pitch;
            memory.yawBase = memory.yaw;
            break;

        case 2:
            float dx = -(x - memory.originX);
            float dy = y - memory.originY;
            glm::vec3 dot = getViewSphere(memory.pitch, memory.yaw);
            dot *= 0.1;
            memory.ball.modelMat = glm::mat4(1.0f);
//            memory.ball.modelMat = glm::translate(memory.ball.modelMat, glm::vec3(dx, dy, 0.0f));
            memory.ball.modelMat = glm::translate(memory.ball.modelMat, dot);
            memory.yaw = memory.yawBase + dx * 2.0f;
            memory.pitch = memory.pitchBase + dy * 5.0f;
            break;
    }
    if (memory.pitch >= (memory.pi - memory.dx) / 2.0f) {
        memory.pitch = (memory.pi - memory.dx) / 2.0f;
    } else if (memory.pitch <= -(memory.pi - memory.dx) / 2.0f) {
        memory.pitch = -(memory.pi - memory.dx) / 2.0f;
    }

//    LOG("Action: %d %f %f. Pitch & Yaw: %f %f", action, x, y, memory.pitch, memory.yaw);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    LOG("Version %s", glGetString(GL_VERSION));
    memory.poolProgram = linkProgramByPath(
            "shaders/generic.vertex.glsl",
            "shaders/pool.fragment.glsl",
            resources
    );
    memory.ballProgram = linkProgramByPath(
            "shaders/billboard.vertex.glsl",
            "shaders/balls.fragment.glsl",
            resources
    );

    // === GL CONFIG === //
    glEnable(GL_DEPTH_TEST);
    memory.lastInstant = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // === MODELS === //
    memory.pool = Model(resources, "models/Pool.model");
    memory.pool.setRenderWireframe(true);
    memory.ball = Model(resources, "models/Ball.model");
    memory.hole = Model(resources, "models/Hole.model");

    glm::vec3 holePositions[] = {
            { 0.0f, 0.0f, 0.0f },
            { 0.5f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.5f },
            { 0.5f, 0.0f, 0.5f },
            { 0.0f, 1.0f, 0.0f },
            { 0.5f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.5f },
            { 0.5f, 1.0f, 0.5f },
    };
    for (const glm::vec3 holePos : holePositions) {
        Entity holeEntity(&memory.hole);
        holeEntity.position = holePos;
        holeEntity.billboard = true;
        memory.holes.push_back(holeEntity);
    }


    // === CAMERAS === //
    memory.front = glm::vec3(0.0f, 0.0f, -1.0f);
    memory.up = glm::vec3(0.0f, 1.0f, 0.0f);
    memory.origin = glm::vec3(0.15f, 0.7f, 2.0f);
    memory.pitch = 0.0f;
    memory.yaw = 0.0f;

    // === MATHY STUFFS === //
    memory.pi = NK_PI;
    memory.dx = 0.0001f;

    // === TESTS === //
    memory.testModel = Model(resources, "models/TestCube.model");
    memory.rotationDegree = 0.0f;
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_surfaceChanged(JNIEnv *env, jobject instance, jint w,
                                                              jint h) {
    memory.aspectRatio = (float) w / h;
    memory.perspectiveMat = glm::perspective(45.0f, memory.aspectRatio, 0.1f, 100.0f);
}

extern "C"
JNIEXPORT void JNICALL
Java_aiofwa_org_a3dsnooker_game_EngineRenderer_render(JNIEnv *env, jobject instance) {
    long long int thisInstant = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
    memory.deltaTime = (float) (thisInstant - memory.lastInstant) / 1000.0f;
    memory.lastInstant = thisInstant;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // === TESTS === //
    // memory.testModel.render(memory.prog, memory);
    // memory.rotationDegree += memory.deltaTime * 1.0f;

    // === CAMERA === //
    memory.viewMat = glm::lookAt(
            getViewSphere(memory.pitch, memory.yaw),
            glm::vec3(0.25f, 0.5f, 0.25f),
            memory.up
    );

    memory.poolProgram.use();
    memory.poolProgram.configureCamera(memory.viewMat, memory.perspectiveMat);
    memory.pool.render(memory.poolProgram, memory);

    memory.ballProgram.use();
    memory.poolProgram.configureCamera(memory.viewMat, memory.perspectiveMat);
    for (int i = 0; i < memory.holes.size(); i++) {
        Entity &hole = memory.holes[i];
        hole.render(memory.ballProgram, memory);
    }
    memory.ball.render(memory.ballProgram, memory);

    // === UI === //

}
