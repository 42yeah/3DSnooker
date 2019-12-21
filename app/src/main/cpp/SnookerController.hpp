//
// Created by apple on 2019-12-21.
//

#ifndef SNOOKER3D_MOBILE_SNOOKERCONTROLLER_HPP
#define SNOOKER3D_MOBILE_SNOOKERCONTROLLER_HPP

#include "Ext/glm/glm.hpp"
#include "Entity.hpp"


class Snooker;

class SnookerController {
public:
    SnookerController() {}
    SnookerController(Snooker *snookerGame, EntityType ballType);

    virtual void notifyTurnArrived() {};
    virtual void process() = 0;

protected:
    Snooker *instance;
    EntityType ballType;
};

class PlayerSnookerController : public SnookerController {
public:
    PlayerSnookerController(Snooker *snookerGame, EntityType ballType);

    virtual void process() override;

private:
    bool previousFingerDown;
    glm::vec2 fingerPosWhenDown;
};

class NPSnookerController : public SnookerController {
public:
    NPSnookerController(Snooker *snookerGame, EntityType ballType);

    virtual void notifyTurnArrived() override;
    virtual void process() override;

private:
    float expectedRotation;
    float expectedForce;
    bool processingTurn;
};

#endif //SNOOKER3D_MOBILE_SNOOKERCONTROLLER_HPP
