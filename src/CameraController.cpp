//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "CameraController.h"

CameraController::CameraController(kick::GameObject *gameObject, kick::Camera * mainCamera, BakeCamera * bakeCamera, kick::MeshRenderer* meshRenderer)
        : Component(gameObject), mainCamera(mainCamera), bakeCamera(bakeCamera), meshRenderer(meshRenderer)
{
}

void CameraController::update() {
    if (frame > -1){
        // save result

    }
    frame++;

}
