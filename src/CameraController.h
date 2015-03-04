#pragma once

#include "kick/kick.h"
#include "BakeCamera.h"
#include "DepthBakeArgs.h"
#include "ViewAngle.h"

class CameraController : public kick::Component , public kick::Updatable {
    
    kick::Camera * mainCamera = nullptr;
    BakeCamera * bakeCamera = nullptr;
    kick::MeshRenderer* meshRenderer;
	std::shared_ptr<DepthBakeArgs> args;
	int elevationRotations;
public:
	int frame = -1;
	ViewAngle viewAngle;
	bool plane = false;
	kick::MeshRenderer* planeMesh;
    CameraController(kick::GameObject *gameObject, kick::Camera * mainCamera, BakeCamera * bakeCamera, kick::MeshRenderer* meshRenderer, std::shared_ptr<DepthBakeArgs> args);

	std::vector<ViewAngle> viewAngles;

    virtual void update();


};