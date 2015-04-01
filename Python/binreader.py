# -*- coding: utf-8 -*-
"""
Created on Mon Oct 06 15:40:20 2014

Title: binreader - Read binary data files

@author: dmha
"""

import numpy as np
import struct

def readbin(path, height, width, skip=0, fmt='float',endian='>'):
    fid = open(path, 'rb')
    fid.seek(skip)
    if fmt == 'float':
        img = fid.read(height*width*4)
        # determine how many floats there are
        count = len(img) // 4
        # unpack the data as floats
        img = np.array(struct.unpack_from(endian+'{0}f'.format(count), img))
                                                        # one big structure of `count` floats
                                                        # results returned as a tuple
    elif fmt == 'uint':
        img = fid.read(height*width*2)
        # determine how many unsigned integers there are
        count = len(img) // 2
        # unpack the data as unsigned integers
        img = np.array(struct.unpack_from(endian+'{0}H'.format(count), img))
                                                        # one big structure of `count` int
                                                        # results returned as a tuple
    elif fmt == 'byte':
        img = fid.read(height*width)
        # determine how many unsigned integers there are
        count = len(img)
        # unpack the data as bytes
        img = np.array(struct.unpack_from(endian+'{0}B'.format(count), img))
                                                        # one big structure of `count` int
    elif fmt == 'long':
        img = fid.read(height*width*4)
        # determine how many unsigned integers there are
        count = len(img) // 4
        # unpack the data as bytes
        img = np.array(struct.unpack_from(endian+'{0}l'.format(count), img))
                                                        # one big structure of `count` int
                                                        # results returned as a tuple                                                        # results returned as a tuple
    else:
        img = np.empty(height,width)
    
    fid.close()
    return(img)
    
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
    
    if  hdr.rfind('TargetType= ') == -1:    #Image is a clutter view
        img_mag = readbin(path, rows, cols, hdr_length, 'uint').reshape(rows,cols)
        indx = hdr.rfind('SensorCalibrationFactor= ') + 25
        calib = float(hdr[indx:indx+hdr[indx:-1].find('\n')])    
        img_mag = img_mag * calib/65536 
        target = 'clutter'
        img_phase = readbin(path, rows, cols, hdr_length+rows*cols*2, 'uint').reshape(rows,cols)
        img_phase = img_phase * calib/65536
    
    else:                                   #Image is a Target view
        img_mag = readbin(path, rows, cols, hdr_length).reshape(rows,cols)
        indx = hdr.rfind('TargetType= ') + 12
        target = hdr[indx:indx+hdr[indx:-1].find('\n')]
        img_phase = readbin(path, rows, cols, hdr_length+rows*cols*4).reshape(rows,cols)   
        
    return(img_mag, img_phase, hdr)
    
def readNORB(path, fromto = [0], order='stack'): 
#NORB data is stored in one file, indexes defines which and how many instances to return.
#Each instance is a image pair, of height*width pixels
    
    #A data file    
    if path.endswith('dat.mat'):
        header = readbin(path, 24, 1, 0, 'byte', '<')

        if len(fromto)<2:
            fromto = [fromto[0], header[9]*256+header[8]]
        
        if order=='stack':      #returns images in a height x width x #_of_images matrix
            images = readbin(path, header[16], header[12]*header[20]*(fromto[1]-fromto[0]), 
             24+fromto[0]*header[16]*header[20]
             , 'byte','<').reshape((fromto[1]-fromto[0])*header[12],header[16],header[20]) 
        elif order=='collate':  #returns images in a (height x width) x #_of_images matrix
            images = readbin(path, header[16], header[20]*(fromto[1]-fromto[0])*header[2], 
             24+fromto[0]*header[16]*header[20]
             , 'byte','<').reshape((fromto[1]-fromto[0])*header[12],header[16]*header[20]) 

    #A category file - Telling in whict group the target belongs
    elif path.endswith('cat.mat'):
        header = readbin(path, 20, 1, 0, 'byte', '<')
        if len(fromto)<2:
            fromto = [fromto[0], header[9]*256+header[8]]
            
        images = readbin(path, 1, header[16]*(fromto[1]-fromto[0]), 
         20+fromto[0]*header[16], 'long','<') 
         
    #An information file - Containing specific information on image from data file
    else:
        header = readbin(path, 20, 1, 0, 'byte', '<')
        if len(fromto)<2:
            fromto = [fromto[0], header[9]*256+header[8]]
            
        images = readbin(path, header[12], fromto[1]-fromto[0], 
         20+fromto[0]*header[16], 'long','<').reshape(fromto[1]-fromto[0],header[12])       

    return(images)