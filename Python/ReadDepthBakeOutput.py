# -*- coding: utf-8 -*-
"""
Created on Mon Dec 22 14:56:51 2014

@author: dmha

Read DeptBake output
"""

import numpy as np
import binreader as bin
import matplotlib.pyplot as plt
from numpy import log10

#target = np.genfromtxt('C:\Users\DMHA\Documents\Visual Studio 2013\Projects\DepthBake\\test.raw', delimiter=',')
def ReadDepthBakeOutput(targetPath, planePath, dataPath, outputFilename):
    target = np.genfromtxt(targetPath, delimiter=',')
    plane = np.genfromtxt(planePath, delimiter=',')
    [im, _, hdr] = bin.readMSTAR(dataPath)

    imh = np.size(im,0)
    imw = np.size(im,1)
    ma = np.max(target)
    mi = np.min(target[target>0.0])
    img=20*log10(abs(im)+1)
    plt.figure()
    plt.imshow(img,'gray')
    plt.clim(0.0,1)

    ind=hdr.find('RangePixelSpacing')+len('RangePixelSpacing')+2
    step_range = float(hdr[ind:ind+hdr[ind:].find('\n')])
    ind=hdr.find('CrossRangePixelSpacing')+len('CrossRangePixelSpacing')+2
    step_cross = float(hdr[ind:ind+hdr[ind:].find('\n')])

    kfactor = round(1 / (step_cross / (20./512) - np.floor(step_cross / (20./512))))
    columns=np.sum(target,0).nonzero()[0]
    shadowlen = np.zeros(len(columns))
    spos = np.zeros(len(columns))
    targetlen = np.zeros(len(columns))

    #Find Starting position of object and shadow plus the length of object and shadow
    for i in range(len(columns)):                                    #columns with object
        shadowpos = max(target[:,columns[i]].nonzero()[0])
        shadowlen[i] = round((plane[shadowpos,columns[i]]-target[shadowpos,columns[i]])/step_range)
        targetlen[i] = round((max(target[:,columns[i]]) - min(target[target[:,columns[i]]>0,columns[i]])) / step_range)
        spos[i] = round((min(target[target[:,columns[i]]>0,columns[i]]) - mi) / step_range)

    objekt = np.zeros([max(spos+targetlen+shadowlen),len(columns)])
    for i in range(len(columns)):
        objekt[spos[i]:spos[i]+targetlen[i],i]=1
        objekt[spos[i]+targetlen[i]:spos[i]+targetlen[i]+shadowlen[i],i]=2

    #create a new array where an even number (div) of columns corresponds to one column in SAR image regarding metric units.
    div = kfactor * step_cross/(20./512)
    kron = np.kron(objekt,np.ones(kfactor))

    #now when div columns in kron correspons to one in our SAR image we can find the outline of the object
    seg = np.zeros([np.size(kron,0),np.round(np.size(kron,1)/div)])
    for i in range(int(np.size(kron,1)/div)):
        seg[:,i] = np.amax(kron[:,i*div:(i+1)*div],1)
    seg = np.flipud(seg)

    #calculate where to place our segmentated object
    sc = imw/2 - int(np.where(columns==255)[0]*5/div)
    [x,y] = np.where(seg==1)
    shadowheight = np.min(x)
    x = x-shadowheight
    k=np.zeros(imh-len(np.unique(x)))
    for i in range(np.size(im,0)-len(np.unique(x))):
        k[i]=np.sum(im[x+i,y+sc])
    sr = np.argmax(k)-shadowheight

    #seg's indicies +sr in rows and +sc in columns is the outline of the tank
    SARseg = np.zeros([np.size(im,0),np.size(im,1)])
    SARseg[0+sr:np.size(seg,0)+sr,0+sc:np.size(seg,1)+sc] = seg

    plt.scatter(np.where(SARseg==1)[1],np.where(SARseg==1)[0],c='b',s=20)
    plt.scatter(np.where(SARseg==2)[1],np.where(SARseg==2)[0],c='r',s=20)
    plt.axis([ 0, 171, 172, 0])
    #numpy write
    np.savetxt(outputFilename+".csv",SARseg, delimiter=',')
    plt.savefig(outputFilename+".png")
    plt.close()