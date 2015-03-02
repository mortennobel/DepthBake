#pragma once

#include "kick/kick.h"
#include <string>

class CameraController;

class BakeCamera : public kick::CameraOrthographic {
    int screenWidth;
    std::shared_ptr<kick::Texture2D> texture;
public:
	CameraController * cameraController;
    // filename with no file ending
    std::string filename = "test";
    bool writeDebugBMP = true;

    BakeCamera(kick::GameObject *gameObject, int screenWidth);

    virtual void render(kick::EngineUniforms *engineUniforms) override;
};