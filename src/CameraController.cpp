//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "CameraController.h"
#include <iostream>
#include <sstream> 

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
		if (f >= viewAngles.size()){
			cout << "Finished" << endl;
			exit(0);
		}
		viewAngle = viewAngles[f];
		
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

		glm::vec3 cameraPos(0, 0, -args->cameraRadius);

		cameraPos = glm::rotateX(cameraPos, -glm::radians((float)viewAngle.getElevation()));
		cameraPos = glm::rotateY(cameraPos, -glm::radians((float)viewAngle.getAzimuth()+90));

		std::ostringstream fname;
		fname << "depthbake_ elevation " << viewAngle.getElevation() << " azimuth "<<viewAngle.getAzimuth() << "_plane" << plane ;


		kick::Engine::context()->setWindowTitle(fname.str());

		// save result
		mainCamera->gameObject()->transform()->setLocalPosition(cameraPos);
		mainCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });

		bakeCamera->gameObject()->transform()->setLocalPosition(cameraPos);
		bakeCamera->gameObject()->transform()->lookAt(glm::vec3{ 0 });
    }
    frame++;

}
