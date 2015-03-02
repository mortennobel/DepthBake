//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "CameraController.h"
#include <iostream>

using namespace std;

CameraController::CameraController(kick::GameObject *gameObject, kick::Camera * mainCamera, BakeCamera * bakeCamera, kick::MeshRenderer* meshRenderer)
        : Component(gameObject), mainCamera(mainCamera), bakeCamera(bakeCamera), meshRenderer(meshRenderer)
{
}

void CameraController::update() {
    if (frame > -1){
        // save result
		mainCamera->gameObject()->transform()->setLocalPosition({ 0, .25f, 1.5 });
		mainCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });


		bakeCamera->gameObject()->transform()->setLocalPosition({ 0, .25f, 1.5 });
		bakeCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });
    }
    frame++;

}
