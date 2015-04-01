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
#include <fstream>
#include "LoadViewAngle.h"

using namespace kick;

const float inchToMeter = 0.0254f;
const float mmToMeter = 0.001f;


void convertFromInchesToMeters(Scene* scene, float scale){
	
	for (auto & go : *scene){
		if (go->transform()->localPosition() != glm::vec3(0)){
			go->transform()->setLocalPosition(go->transform()->localPosition()* scale);
		}
	}

	std::vector<MeshRenderer*> meshes = scene->findComponents<MeshRenderer>();
	for (auto mr : meshes){ 
		auto meshData = mr->mesh()->meshData();
		std::vector<glm::vec3> newPos;
		for (auto p : meshData->position()){
			newPos.push_back(p * scale);
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
	std::vector<ViewAngle> viewAngles;
	if (arg->viewAngleFile.length() > 0){
		viewAngles = loadViewAngleFile(arg->viewAngleFile);
	}

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
	
	float scale = arg->inputUnit == "inch" ? inchToMeter : mmToMeter;

	convertFromInchesToMeters(scene, scale);

	// Find all mesh renderers and determine bounds
	std::vector<MeshRenderer*> meshes = scene->findComponents<MeshRenderer>();
	std::set<Transform*> rootTransforms;
	for (auto m : meshes){
		rootTransforms.insert(m->transform()->root());
	}
	
	// create new root and offset root
	auto meshRootGO = scene->createGameObject("MeshRoot");
	if (arg->flipYZ){
		meshRootGO->transform()->setRotationEuler({ -glm::pi<float>() / 2, 0, 0 });
	}
	for (auto rt : rootTransforms){
		rt->setParent(meshRootGO->transform());
	}
	auto bounds = meshes[0]->mesh()->meshData()->bounds();
	for (auto m : meshes){
		auto meshBounds = m->mesh()->meshData()->bounds();
		meshBounds = meshBounds.transform(m->transform()->globalMatrix());
		bounds.expand(meshBounds.max);
		bounds.expand(meshBounds.min);
	}
	std::cout << "Bounds " << bounds << std::endl;
	std::cout << "Bounds size " << glm::to_string(bounds.diagonal()) << std::endl;
	
	auto offset = -bounds.center();
	offset.y = -bounds.min.y;
	meshRootGO->transform()->setLocalPosition(offset);
	
    auto cameraGameObject = scene->createGameObject("mainCamera");
    auto mainCamera = cameraGameObject->addComponent<CameraOrthographic>();
	mainCamera->setLeft(-arg->cameraResolution*0.5f);
	mainCamera->setRight(arg->cameraResolution*0.5f);
	mainCamera->setTop(arg->cameraResolution*0.5f);
	mainCamera->setBottom(-arg->cameraResolution*0.5f);
	mainCamera->setNear(0);
	mainCamera->setFar(arg->cameraRadius*100);

    GameObject * gameObject = scene->createGameObject();
    BakeCamera * bakeCamera = gameObject->addComponent<BakeCamera>(screenWidth, arg->outputPath);
	bakeCamera->setLeft(-arg->cameraResolution*0.5f);
	bakeCamera->setRight(arg->cameraResolution*0.5f);
	bakeCamera->setTop(arg->cameraResolution*0.5f);
	bakeCamera->setBottom(-arg->cameraResolution*0.5f);
	bakeCamera->setNear(0);
	bakeCamera->setFar(arg->cameraRadius * 100);
    CameraController * cameraController = gameObject->addComponent<CameraController>(mainCamera, bakeCamera, nullptr, arg);
	bakeCamera->cameraController = cameraController;

	if (arg->viewAngleFile.length() > 0){
		cameraController->viewAngles = viewAngles;
	}

    auto shader = Project::loadShader("custom.shader");
    auto material = std::make_shared<Material>();
    material->setShader(shader);
	bakeCamera->setReplacementMaterial(material);
	
	auto light = scene->createDirectionalLight();
	light->transform()->setLocalRotationEuler({0.1,0.1,0.1});

	mainCamera->setReplacementMaterial(material);

    MeshRenderer* plane = scene->createCube(nullptr,1.0f);
    plane->transform()->setLocalScale(glm::vec3(100000.0f,0.0001f,100000.0f));
    
	cameraController->planeMesh = plane;

    scene->createDirectionalLight();
    Engine::startMainLoop();

    return 0;
}
