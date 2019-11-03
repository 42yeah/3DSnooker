//
// Created by 周昊 on 2019/11/2.
//

#include "model.h"
#include "files.h"
#include <sstream>
#include <vector>
#include "memory.h"


// === CONSTRUCTORS === //
Model::Model(Resources *res, std::string path) : modelMat(glm::mat4(1.0f)) {
    importFromFile(res, path);
}

Model::Model(float *buffer, int n) : modelMat(glm::mat4(1.0f)) {
    this->vertices.insert(this->vertices.end(), buffer, buffer + n);
    this->numVertices = n / AXISES;
    this->numIndices = 0;
}

// === METHODS === //
void Model::importFromFile(Resources *res, std::string path) {
    this->vertices.clear();
    this->indices.clear();
    std::stringstream ss = res->readFileAsStringStream(path);
    ss.seekg(0, ss.beg);
    int n;
    ss >> n;
    for (int i = 0; i < n; i++) {
        float f;
        ss >> f;
        this->vertices.push_back(f);
    }

    this->numVertices = n;
    ss >> n;
    for (int i = 0; i < n; i++) {
        unsigned int u;
        ss >> u;
        this->indices.push_back(u);
    }
    this->numIndices = n;
    if (numIndices == 0) {
        numVertices /= AXISES;
    }
    LOG("Model %s loaded. numVertices: %d, numIndices: %d", path.c_str(), this->numVertices, this->numIndices);
}

void Model::render(Program &prog, Memory &mem) {
    // Render trusts that camera has already been set up.
    prog.use();
    glEnableVertexAttribArray(prog.aPosLocation);
    glEnableVertexAttribArray(prog.aColorLocation);
    glEnableVertexAttribArray(prog.aUVLocation);
    glVertexAttribPointer(prog.aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, &this->vertices[0]);
    glVertexAttribPointer(prog.aColorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, &this->vertices[3]);
    glVertexAttribPointer(prog.aUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, &this->vertices[6]);
    glUniformMatrix4fv(prog.modelLocation, 1, GL_FALSE, value_ptr(this->modelMat));
    if (this->numIndices > 0) {
        if (renderWireframe) {
            glDisable(GL_DEPTH_TEST);
            glDrawElements(GL_LINES, this->numIndices, GL_UNSIGNED_INT, &this->indices[0]);
            glEnable(GL_DEPTH_TEST);
        } else {
            glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, &this->indices[0]);
        }
    } else {
        if (renderWireframe) {
            glDisable(GL_DEPTH_TEST);
            glDrawArrays(GL_LINES, 0, this->numVertices);
            glEnable(GL_DEPTH_TEST);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
        }
    }

}

void Model::setRenderWireframe(bool wireframe) {
    this->renderWireframe = wireframe;
}

// === ENTITY === //
// === CONSTURCTORS === //
Entity::Entity(Model &model) {
    this->flyweight = &model;
}

// === METHODS === //
void Entity::update(float dt) {
    LOG("Update: Stub!");
}

void Entity::render(Program &prog, Memory &mem) {
    this->flyweight->modelMat = glm::mat4(1.0f);
    this->flyweight->modelMat = glm::translate(
            this->flyweight->modelMat,
            this->position
    );
    this->flyweight->setRenderWireframe(renderWireframe);
    this->flyweight->render(prog, mem);
}
