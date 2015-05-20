#pragma once

#include "kick/kick.h"
#include <string>

class CameraController;

class BakeCamera : public kick::CameraOrthographic {
    int screenWidth;
//    std::shared_ptr<kick::Texture2D> texture;
    std::shared_ptr<kick::Texture2D> textureDepth;
    std::string outputPath;
public:
	std::shared_ptr<CameraController> cameraController;

    BakeCamera(kick::GameObject *gameObject, int screenWidth, std::string outputPath);

    virtual void render(kick::EngineUniforms *engineUniforms) override;
};