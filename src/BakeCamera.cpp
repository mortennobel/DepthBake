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
#include "CameraController.h"
#include <sstream> 

using namespace kick;



BakeCamera::BakeCamera(kick::GameObject *gameObject, int screenWidth, std::string outputPath)
        : CameraOrthographic(gameObject),screenWidth(screenWidth), outputPath(outputPath)
{
	printOpenGLError();
    textureDepth = std::make_shared<Texture2D>();
    textureDepth->setEmptyDepthTexture(screenWidth, screenWidth, 32, true);
    TextureRenderTarget *renderTarget = new TextureRenderTarget();
    renderTarget->setDepthTexture(textureDepth);
    renderTarget->apply();
    printOpenGLError();
    setTarget(renderTarget);
	setIndex(2);
}

void BakeCamera::render(EngineUniforms *engineUniforms) {
    CameraOrthographic::render(engineUniforms);
	glFlush();
	glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);

    auto dataDepth = textureDepth->data();

    for (int x=0;x<textureDepth->width();x++){
        for (int y=0;y<textureDepth->height();y++) {
            float depth = near() + (far()- near())* dataDepth.vec1(x,y);
            if (depth == far()){
                depth = 0; // workaround mark as unused (0 distance)
            }
            dataDepth.vec1(x,y) = depth;
        }
    }

	std::ostringstream fname;
    if (cameraController->viewAngle.getFilename().length() == 0){
        return;
    }
	fname << outputPath << "depthbake_"<<cameraController->viewAngle.getFilename() << "_plane" << cameraController->plane << ".raw";

    float* resFloat = &(dataDepth.vec1(0,0));

	std::ofstream fileOut(fname.str());
    for (int i=0;i<textureDepth->width()*textureDepth->height();i++){
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

    printOpenGLError();
}