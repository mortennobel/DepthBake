#pragma once

#include "kick/kick.h"
#include "BakeCamera.h"


class CameraController : public kick::Component , public kick::Updatable {
    int frame = -1;
    kick::Camera * mainCamera = nullptr;
    BakeCamera * bakeCamera = nullptr;
    kick::MeshRenderer* meshRenderer;
public:
    CameraController(kick::GameObject *gameObject, kick::Camera * mainCamera, BakeCamera * bakeCamera, kick::MeshRenderer* meshRenderer);

    virtual void update();


};