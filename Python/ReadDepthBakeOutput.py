# -*- coding: utf-8 -*-
"""
Created on Mon Dec 22 14:56:51 2014

@author: dmha

Read DeptBake output
"""
import matplotlib
matplotlib.use('Agg')
import os, sys
sys.path.append(os.path.abspath("../../modules")) 
import matplotlib.pyplot as plt
import numpy as np
import scipy.ndimage as ndim
import scipy.signal
import binreader as bin
from numpy import log10
from mitplotlib import boundary_plot
    
def ReadDepthBakeOutput(targetPath, planePath, dataPath, outputFilename):
    print(targetPath+ " # "+ planePath)
    target = np.genfromtxt(targetPath, delimiter=',')
    plane = np.genfromtxt(planePath, delimiter=',')
    [im, _, hdr] = bin.readMSTAR(dataPath)

    imh = np.size(im,0)
    imw = np.size(im,1)
    img=20*log10(abs(im)+1)
    plt.figure()
    plt.imshow(img,'gray')
    plt.clim(0.0,1)

    #### Automatic loook for pixel spacing in header ####
    ind=hdr.find('RangePixelSpacing')+len('RangePixelSpacing')+2
    step_range = float(hdr[ind:ind+hdr[ind:].find('\n')])
    ind=hdr.find('CrossRangePixelSpacing')+len('CrossRangePixelSpacing')+2
    step_cross = float(hdr[ind:ind+hdr[ind:].find('\n')])
    
    shadow_=np.zeros([np.size(plane,0),np.size(plane,1)])
    tbl=target.nonzero()
    (y,x)=tbl
    shadow_[y,x]=1
    shadow_=ndim.binary_fill_holes(shadow_).astype(int)
    
    DBres = (20./512)
    kfactor = round(1 / (step_cross / DBres - np.floor(step_cross / DBres)))
    cols_idx = np.sum(target,0).nonzero()[0]
    max_tar = np.max(target)
    min_tar = np.min(target[target>0.0])
    
    target[y,x] = target[y,x]-min_tar
    plane = plane-min_tar
    max_sha = np.max(plane[y,x])
    
    segDB = np.zeros([round((max_sha) / step_range), len(cols_idx)])
    shaDB = np.zeros([round((max_sha) / step_range), len(cols_idx)])
    
    #### Running through each column and find target depths and shadow lines ####
    for i in range(len(cols_idx)):  
        tar_depths = target[target[:,cols_idx[i]].nonzero()[0],cols_idx[i]]                            #columns with object
        tar_idx = np.floor((tar_depths) / step_range).astype(int)
        segDB[tar_idx,i] = 1
        
        vect = shadow_[:,cols_idx[i]]
        diff = vect[1:]-vect[0:-1]
        idx = diff.nonzero()[0]+1
        
        for j in range(len(idx)/2):
            fr = np.floor(( plane[idx[j*2],cols_idx[i]]) / step_range).astype(int)
            to = np.floor(( plane[idx[(j+1)*2-1],cols_idx[i]]) / step_range).astype(int)
            shaDB[fr:to,i] = 2
      
    
    
    #### Extending matrix with a kroenecker function is a way of resampling to the correct pixel size ####
    div = kfactor * step_cross/(20./512)
    krontar = np.kron(segDB,np.ones(kfactor)) 
    kronsha = np.kron(shaDB,np.ones(kfactor)) 
    segDB = np.zeros([np.size(krontar,0),np.round(np.size(krontar,1)/div)])
    shaDB = np.zeros([np.size(kronsha,0),np.round(np.size(kronsha,1)/div)])
    for i in range(int(np.size(kronsha,1)/div)):
        segDB[:,i] = np.amax(krontar[:,i*div:(i+1)*div],1)
        shaDB[:,i] = np.amax(kronsha[:,i*div:(i+1)*div],1)
    
    seg=np.array(shaDB)
    tbl=segDB.nonzero()
    (y,x)=tbl
    seg[y,x]=1
    
    seg = np.flipud(seg)
    
    ##calculate where to place our segmentated object
    temp=np.array(seg)
    temp[np.where(temp==2)]=0
    corr=scipy.signal.correlate2d(im,temp,mode='same')
    max_idx=np.argmax(corr)
    from_row = max_idx/imw - np.size(seg,0)/2
    from_col = max_idx%imw - np.size(seg,1)/2# - int(np.where(columns==255)[0]*5/div)
    
    #seg's indicies +sr in rows and +sc in columns is the outline of the tank
    SARseg = np.zeros([np.size(im,0),np.size(im,1)])
    to_row = np.size(seg,0)+from_row
    to_col = np.size(seg,1)+from_col    
    
    cut_row = 0     #This is a hack to handle if segmentation is out of image range
    cut_col = 0
    if from_row < 0: 
        cut_row = abs(from_row)
        from_row = 0
    if from_col < 0:
        cut_col = abs(from_col)
        from_col = 0
        
    SARseg[from_row:to_row,from_col:to_col] = seg[cut_row:,cut_col:]

    boundary_plot(SARseg)
    plt.axis([ 0, np.size(SARseg,0), np.size(SARseg,1), 0])
    
    #numpy write
    np.savetxt(outputFilename+".csv",SARseg.astype('int'), delimiter=',',fmt='%d')
    plt.savefig(outputFilename+".png")
    plt.close()