//
// Created by Morten Nobel-Jørgensen on 22/12/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//

#include "BakeCamera.h"
#ifdef _WIN32
#include <SDL_surface.h>
#else
#include <SDL2/SDL_surface.h>
#endif
#include <fstream>

using namespace kick;

BakeCamera::BakeCamera(kick::GameObject *gameObject, int screenWidth)
        : CameraOrthographic(gameObject),screenWidth(screenWidth)
{
	printOpenGLError();
    texture = std::make_shared<Texture2D>();
    ImageFormat imageFormat;
    imageFormat.internalFormat = GL_R32F;
    imageFormat.format = GL_RED;
    imageFormat.mipmap = Mipmap::None;
    imageFormat.type = GL_FLOAT;
    texture->setData(screenWidth, screenWidth, nullptr, imageFormat);
    TextureRenderTarget *renderTarget = new TextureRenderTarget();
    renderTarget->setColorTexture(0, texture);
    renderTarget->apply();
    printOpenGLError();
    setTarget(renderTarget);
	this->setIndex(2);
}

void BakeCamera::render(EngineUniforms *engineUniforms) {
    CameraOrthographic::render(engineUniforms);
	glFlush();
	glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    // grap result
    texture->bind(1);

    //glGetTexImage()
    //texture->saveBMPImage("test.bmp");
    std::vector<char> res(screenWidth * screenWidth *4);
    std::vector<float> resFloat(screenWidth * screenWidth);
	
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, resFloat.data());
    for (int i=0;i<res.size();i++){
        res[i] = (char) (resFloat[i/4]*255);
    }

    std::ofstream fileOut(filename+".raw");
    for (int i=0;i<resFloat.size();i++){
        if (i>0){
            if (i%screenWidth == 0){
                fileOut << "\n";
            } else {
                fileOut << ",";
            }
        }
        fileOut << resFloat[i];
    }
	fileOut << std::flush;


    if (writeDebugBMP){
        SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceFrom(res.data(), screenWidth, screenWidth,32, screenWidth *4, 0xff,0xff<<8,0xff<<16,0xff<<24);
        std::string bmpFilename = filename + ".bmp";
        SDL_SaveBMP(sdlSurface, bmpFilename.c_str());
        sdlSurface->pixels = nullptr;
        SDL_FreeSurface(sdlSurface);
    }
    printOpenGLError();
}