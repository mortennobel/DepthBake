//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "BakeCamera.h"

using namespace kick;

BakeCamera::BakeCamera(kick::GameObject *gameObject, int screenWidth)
        : CameraOrthographic(gameObject)
{
    texture = std::make_shared<Texture2D>();
    ImageFormat imageFormat;
//    imageFormat.format = GL_RGB;
//    imageFormat.internalFormat = GL_RGBA32F;
    imageFormat.mipmap = Mipmap::None;
//    imageFormat.type = GL_FLOAT;
    texture->setData(screenWidth, screenWidth, nullptr, imageFormat);
    TextureRenderTarget *renderTarget = new TextureRenderTarget();
    renderTarget->setColorTexture(0, texture);
    renderTarget->apply();
    printOpenGLError();
    setTarget(renderTarget);
}

void BakeCamera::render(EngineUniforms *engineUniforms) {
    CameraOrthographic::render(engineUniforms);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    // grap result
    texture->bind(1);

    //glGetTexImage()
    texture->saveBMPImage("test.bmp");
}
