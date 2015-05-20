#pragma once

#include "kick/kick.h"
#include "BakeCamera.h"
#include "DepthBakeArgs.h"
#include "ViewAngle.h"

class CameraController : public kick::Component , public kick::Updatable {

    std::shared_ptr<kick::Camera> mainCamera = nullptr;
    std::shared_ptr<BakeCamera>  bakeCamera = nullptr;
    std::shared_ptr<kick::MeshRenderer> meshRenderer;
	std::shared_ptr<DepthBakeArgs> args;
	int elevationRotations;
public:
	int frame = -1;
	ViewAngle viewAngle;
	bool plane = false;
	std::shared_ptr<kick::MeshRenderer> planeMesh;
    CameraController(kick::GameObject* gameObject, std::shared_ptr<kick::Camera> mainCamera, std::shared_ptr<BakeCamera> bakeCamera, std::shared_ptr<kick::MeshRenderer> meshRenderer, std::shared_ptr<DepthBakeArgs> args);

	std::vector<ViewAngle> viewAngles;

    virtual void update();


};