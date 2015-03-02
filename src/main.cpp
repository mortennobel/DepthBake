//
//  main.cpp
//  DepthBake
//
//  Created by Morten Nobel-Jørgensen on 16/12/14.
//  Copyright (c) 2014 Morten Nobel-Jørgensen. All rights reserved.
//

#include <iostream>
#include "kick/kick.h"
#include "kick/assimp/assimp.h"
#include "BakeCamera.h"
#include "CameraController.h"
#include <algorithm>
#include "DepthBakeArgs.h"

using namespace kick;

void convertFromInchesToMeters(Scene* scene){
	const float inchToMeter = 0.0254f;
	for (auto & go : *scene){
		if (go->transform()->localPosition() != glm::vec3(0)){
			go->transform()->setLocalPosition(go->transform()->localPosition()*inchToMeter);
		}
	}

	std::vector<MeshRenderer*> meshes = scene->findComponents<MeshRenderer>();
	for (auto mr : meshes){ 
		auto meshData = mr->mesh()->meshData();
		std::vector<glm::vec3> newPos;
		for (auto p : meshData->position()){
			newPos.push_back(p * inchToMeter);
		}
		meshData->setPosition(newPos);
		meshData->recomputeBounds();
		mr->mesh()->setMeshData(meshData);
		mr->setMesh(mr->mesh());
	}
	
}

int main(int argc, char * argv[])
{
	auto arg = std::make_shared<DepthBakeArgs>(argc, argv);
	
	bool showPlane = false;

    WindowConfig windowConfig;
	int screenWidth = arg->resolution;
    windowConfig.height = screenWidth;
    windowConfig.width = screenWidth;

    Engine::init(argc, argv, windowConfig);

    auto scene = Engine::activeScene();

	AssImpSettings importSettings;
	importSettings.cameras = false;
	importSettings.lights = false;

	bool importSuccess = AssImp::importData(arg->getModelFilename(), scene, importSettings);
	
	convertFromInchesToMeters(scene);

	// Find all mesh renderers and determine bounds
	std::vector<MeshRenderer*> meshes = scene->findComponents<MeshRenderer>();
	auto bounds = meshes[0]->mesh()->meshData()->bounds();
	std::set<Transform*> rootTransforms;
	for (auto m : meshes){
		bounds.expand(m->mesh()->meshData()->bounds().max);
		bounds.expand(m->mesh()->meshData()->bounds().min);
		rootTransforms.insert(m->transform()->root());
	}
	std::cout << "Bounds " << bounds << std::endl;
	std::cout << "Bounds size " << glm::to_string(bounds.diagonal()) << std::endl;
	
	// create new root and offset root
	auto meshRootGO = scene->createGameObject("MeshRoot");
	auto offset = -bounds.center();
	offset.y = -bounds.min.y;
	meshRootGO->transform()->setLocalPosition(offset);
	for (auto rt : rootTransforms){
		rt->setParent(meshRootGO->transform());
	}

    auto cameraGameObject = scene->createGameObject("mainCamera");
    auto mainCamera = cameraGameObject->addComponent<CameraOrthographic>();
	mainCamera->setLeft(-arg->cameraResolution*0.5f);
	mainCamera->setRight(arg->cameraResolution*0.5f);
	mainCamera->setTop(arg->cameraResolution*0.5f);
	mainCamera->setBottom(-arg->cameraResolution*0.5f);
	mainCamera->setNear(0);
	mainCamera->setFar(arg->cameraRadius*100);

    GameObject * gameObject = scene->createGameObject();
    BakeCamera * bakeCamera = gameObject->addComponent<BakeCamera>(screenWidth);
	bakeCamera->setLeft(-arg->cameraResolution*0.5f);
	bakeCamera->setRight(arg->cameraResolution*0.5f);
	bakeCamera->setTop(arg->cameraResolution*0.5f);
	bakeCamera->setBottom(-arg->cameraResolution*0.5f);
	bakeCamera->setNear(0);
	bakeCamera->setFar(arg->cameraRadius * 100);
    CameraController * cameraController = gameObject->addComponent<CameraController>(mainCamera, bakeCamera, nullptr, arg);
	bakeCamera->cameraController = cameraController;
    auto shader = Project::loadShader("custom.shader");
    auto material = new Material();
    material->setShader(shader);
	for (auto m : meshes){
		m->setMaterial(material);
	}

	
    MeshRenderer* plane = scene->createCube(nullptr,1.0f);
    plane->transform()->setLocalScale(glm::vec3(1000.0f,0.0001f,1000.0f));
    plane->setMaterial(material);

	cameraController->planeMesh = plane;

    scene->createDirectionalLight();
    Engine::startMainLoop();

    return 0;
}
