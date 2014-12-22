//
//  main.cpp
//  DepthBake
//
//  Created by Morten Nobel-Jørgensen on 16/12/14.
//  Copyright (c) 2014 Morten Nobel-Jørgensen. All rights reserved.
//

#include <iostream>
#include "kick/kick.h"
#include "BakeCamera.h"
#include "CameraController.h"
#include <algorithm>

using namespace kick;

void rescaleAndCenter(MeshRenderer *meshRenderer, Bounds3 &bounds);

int main(int argc, char * argv[])
{


    bool showPlane = false;

    WindowConfig windowConfig;
    int screenWidth = 64;
    windowConfig.height = screenWidth;
    windowConfig.width = screenWidth;

    Engine::init(argc, argv, windowConfig);

    auto scene = Engine::activeScene();
    auto cameraGameObject = scene->createGameObject("mainCamera");
    auto mainCamera = cameraGameObject->addComponent<CameraOrthographic>();
    //mainCamera->gameObject()->transform()->setLocalPosition({0,.25f,1.5});

    auto meshRenderer = scene->createCube();
    ObjData data = loadObjData(std::string("."),std::string("test.obj"));
    auto mesh = std::make_shared<Mesh>();
    auto meshData = data.meshData();
    meshData->recomputeBounds();
    Bounds3 bounds = meshData->bounds();
    mesh->setMeshData(meshData);
    meshRenderer->setMesh(mesh);
    rescaleAndCenter(meshRenderer, bounds);
//    mainCamera->gameObject()->transform()->lookAt(glm::vec3{0}, glm::vec3{0,1,0});

    GameObject * gameObject = scene->createGameObject();
    BakeCamera * bakeCamera = gameObject->addComponent<BakeCamera>(screenWidth);
    CameraController * cameraController = gameObject->addComponent<CameraController>(mainCamera, bakeCamera, meshRenderer);

    auto shader = Project::loadShader("custom.shader");
    auto material = new Material();
    material->setShader(shader);
    meshRenderer->setMaterial(material);

    if (showPlane){
        auto plane = scene->createCube(nullptr,1.0f);
        plane->transform()->setLocalScale(glm::vec3(1000.0f,0.0001f,1000.0f));
        plane->setMaterial(material);
    }

    scene->createDirectionalLight();
    Engine::startMainLoop();

    return 0;
}

void rescaleAndCenter(MeshRenderer *meshRenderer, Bounds3 &bounds) {
    float maxBoundingBoxLength = std::max({bounds.dimension().x,bounds.dimension().y,bounds.dimension().z});
    float scale = 1/maxBoundingBoxLength;
    meshRenderer->transform()->setLocalScale(glm::vec3(scale));
    glm::vec3 offset = -bounds.center();
    offset.y = -bounds.min.y; // position on the ground
    meshRenderer->transform()->setPosition(offset * scale);
}