#pragma once

#include "kick/kick.h"

class BakeCamera : public kick::CameraOrthographic {
    std::shared_ptr<kick::Texture2D> texture;
public:


    BakeCamera(kick::GameObject *gameObject, int screenWidth);

    virtual void render(kick::EngineUniforms *engineUniforms) override;
};