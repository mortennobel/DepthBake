//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "CameraController.h"
#include <iostream>

using namespace std;

CameraController::CameraController(kick::GameObject *gameObject, kick::Camera * mainCamera, BakeCamera * bakeCamera, kick::MeshRenderer* meshRenderer, std::shared_ptr<DepthBakeArgs> args)
	: Component(gameObject), mainCamera(mainCamera), bakeCamera(bakeCamera), meshRenderer(meshRenderer), args(args)
{
	elevationRotations = (int)ceil((args->elevationMaxAngle - args->elevationMinAngle) - args->elevationStepSize);
}

void CameraController::update() {
    if (frame > -1){
		int f = frame / 2;
		plane = frame % 2;
		
		// show plane or rest
		std::vector<kick::MeshRenderer*> meshes = kick::Engine::activeScene()->findComponents<kick::MeshRenderer>();
		for (auto m : meshes){
			bool enabled;
			if (plane){
				enabled = (m == planeMesh);
			}
			else {
				enabled = (m != planeMesh);
			}
			m->setEnabled(enabled);
		}

		int step = f % args->azimuthSteps;
		azimuthAngle = step * (360.0f / args->azimuthSteps);
		int estep = f / elevationRotations;
		elevationAngle = args->elevationMinAngle + args->elevationStepSize*estep;

		cout << "Azimuth " << azimuthAngle << " elevationAngle " << elevationAngle << endl;

		if (estep > elevationRotations){
			cout <<"Finished" << endl;
			exit(0);
		}

		glm::vec3 cameraPos(0, 0, -args->cameraRadius);

		cameraPos = glm::rotateX(cameraPos, -glm::radians(elevationAngle));
		cameraPos = glm::rotateY(cameraPos, glm::radians(azimuthAngle));

		// save result
		mainCamera->gameObject()->transform()->setLocalPosition(cameraPos);
		mainCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });

		bakeCamera->gameObject()->transform()->setLocalPosition(cameraPos);
		bakeCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });
    }
    frame++;

}
