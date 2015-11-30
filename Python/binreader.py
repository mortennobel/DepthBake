# -*- coding: utf-8 -*-
"""
Created on Mon Oct 06 15:40:20 2014

Title: binreader - Read binary data files

@author: dmha
"""

import numpy as np
    
def readMSTAR(path):
    fid = open(path, 'r')
    
    hdr = fid.read()
    indx = hdr.find('PhoenixHeaderLength= ') + 21               #'PhoenixHeaderLength= ' is 21 characters
    hdr_length = int(hdr[indx:indx+hdr[indx:-1].find('\n')])
    indx = hdr.find('native_header_length= ') + 22              #'native_header_length= ' is 22
    hdr_length = hdr_length + int(hdr[indx:indx+hdr[indx:-1].find('\n')])
    indx = hdr.find('NumberOfColumns= ') + 17                   #'NumberOfColumns= ' is 17
    cols = int(hdr[indx:indx+hdr[indx:-1].find('\n')])
    indx = hdr.find('NumberOfRows= ') + 14                      #'NumberOfColumns= ' is 14
    rows = int(hdr[indx:indx+hdr[indx:-1].find('\n')])
    fid.close()
    
    calib = 0 
    fid = open(path,'rb')
    if  hdr.rfind('TargetType= ') == -1:    #Image is a clutter view
        fid.seek(hdr_length)
        img_mag = np.fromfile(fid, np.dtype('>u2'), rows*cols).reshape(rows,cols)
        
        indx = hdr.rfind('SensorCalibrationFactor= ') + 25
        calib = float(hdr[indx:indx+hdr[indx:-1].find('\n')])    
        img_mag = img_mag * calib/65536
        img_phase = np.fromfile(fid, np.dtype('>u2'), rows*cols).reshape(rows,cols)
        img_phase = img_phase * calib/65536
    
    else:                                   #Image is a Target view
        fid.seek(hdr_length)
        img_mag = np.fromfile(fid, np.dtype('>f'),rows*cols).reshape(rows,cols)
        img_phase = np.fromfile(fid, np.dtype('>f'), rows*cols).reshape(rows,cols)
    
    fid.close()    
    return(img_mag,img_phase,hdr)
    
def readNORB(path, fromto = [0], order='stack'): 
#NORB data is stored in one file, indexes defines which and how many instances to return.
#Each instance is a image pair, of height*width pixels
    
    fid = open(path,'rb')    #open as binary file!
    #A data file    
    if path.endswith('dat.mat'):
        header = np.fromfile(fid, np.dtype('u1'), 24)
        if len(fromto)<2:
            fromto = [fromto[0], header[9]*256+header[8]]

        count = int(float(header[16])*header[12]*header[20]*(fromto[1]-fromto[0]))        
        fid.seek(fromto[0]*2+24)
        
        data = np.fromfile(fid, np.dtype('u1'), count)
        if order=='stack':      #returns data in a height x width x #_of_data matrix
            data = data.reshape((fromto[1]-fromto[0])*header[12],header[16],header[20]) 
        elif order=='collate':  #returns data in a (height * width) x #_of_data matrix
            data = data.reshape((fromto[1]-fromto[0])*header[12],int(float(header[16])*header[20])) 
        else:
            data = data.reshape((fromto[1]-fromto[0]),header[12],header[16],header[20])
            data=data.swapaxes(0,2)
            data=data.swapaxes(1,3)
            data=data.swapaxes(2,3)

    else:
        header = np.fromfile(fid, np.dtype('u1'), 20)
        if len(fromto)<2:
            fromto = [fromto[0], header[9]*256+header[8]]

        count = header[12]*(fromto[1]-fromto[0])
        fid.seek(fromto[0]*4+20)
        
        data = np.fromfile(fid,np.dtype('i4'),count)
        data = data.reshape(header[9]*256+header[8],header[12])
        
    fid.close()
    return(data)