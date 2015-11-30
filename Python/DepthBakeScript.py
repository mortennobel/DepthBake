import ReadDepthBakeOutput as RDBO
import sys
import os
import subprocess
import csv
from os import listdir
from os.path import isfile, join

argc = len(sys.argv)

depthBakePath = 'DepthBake.exe'
depthBakeCWD = os.getcwd()
dataPath = "Z:/original_images/MSTAR/"

if argc > 1:
    targetPath = sys.argv[1]
if argc > 2:
    planePath = sys.argv[2]
if argc > 3:
    planePath = sys.argv[3]

viewAngleFiles  = [
#                   "viewAngleFiles/D7_15.csv",
#                   "viewAngleFiles/D7_17.csv",
#                   "viewAngleFiles/2S1_15.csv",
#                   "viewAngleFiles/2S1_17.csv",
#                   "viewAngleFiles/BMP2_15.csv"]
                   "viewAngleFiles/BMP2_17.csv"]
#                   "viewAngleFiles/BRDM2_15.csv",
#                   "viewAngleFiles/BRDM2_17.csv",
#                   "viewAngleFiles/BTR60_15.csv",
#                   "viewAngleFiles/BTR60_17.csv",
#                   "viewAngleFiles/BTR70.csv"]
#                   'viewAngleFiles/T62_15.csv',
#                   'viewAngleFiles/T62_17.csv',
#                   'viewAngleFiles/ZIL131_15.csv',
#                   'viewAngleFiles/ZIL131_17.csv',
#                    'viewAngleFiles/ZSU23_15.csv',
#                    'viewAngleFiles/ZSU23_17.csv']
#                    'viewAngleFiles/BMP2_15.csv',
#                    'viewAngleFiles/BMP2_17.csv',
#                   "viewAngleFiles/T72_version1_15.csv"]
#                   "viewAngleFiles/T72_version2_17_test.csv"]
#                   "viewAngleFiles/T72_version2_15.csv"]
#                    'viewAngleFiles/test_BTR60.csv']
                    
modelFiles      = [
#                   "Z:\cad_files\MSTAR_Models/D7[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/2S1[mm].obj",
                   "Z:\cad_files\MSTAR_Models/BMP2[mm].obj"]
#                   "Z:\cad_files\MSTAR_Models/BDRM2[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/BTR60[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/BTR70[mm].obj"]
#                   "Z:\cad_files\MSTAR_Models/T62[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/ZIL131[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/ZSU23-4[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/ZSU23-4[mm].obj"]
#                   "Z:\cad_files\MSTAR_Models/MSTAR_Models/2S1[mm].obj",
#                   "Z:\cad_files\MSTAR_Models/T72_Version1[mm].obj"]
#                   "Z:\cad_files\MSTAR_Models/T72_Version2[mm].obj"]
#                   "Z:\cad_files\MSTAR_Models/BMP2[mm].obj"]

depthBakeOutPaths = [
#                    "output/D7/",
#                    "output/D7/",
#                    "output/2S1/",
#                    "output/2S1/",
                     "output/BMP2/"]
#                     "output/BRDM2/",
#                     "output/BTR60/",
#                     "output/BTR60/",
#                     "output/BTR70/"]
#                     "output/T62/",
#                     "output/T62/",
#                     "output/ZIL131/",
#                     "output/ZIL131/",
#                     "output/ZSU23/",
#                     "output/ZSU23/"]
#                     "output/BMP2/"]
#                     "output/T72version1/"]
#                     "output/T72version2/"]
#                    "output/test2/"]


inputUnits = ["mm"]

def RunDepthBake(depthBakeOutPath, viewAngleFile, modelFile, inputUnit):
    # FNULL = open(os.devnull, 'w')    #use this if you want to suppress output to stdout from the subprocess using stdout=FNULL, stderr=FNULL,
    if not os.path.exists(depthBakeOutPath):
        os.makedirs(depthBakeOutPath)
    args = depthBakePath + " -inputUnit "+inputUnit+" -outputPath " + depthBakeOutPath + " -viewAngleFile " + viewAngleFile + ' '+modelFile
    print(args)
    subprocess.call(args, shell=True, cwd=depthBakeCWD)

def AllRunDepthBake():
    for i in range(0,len(viewAngleFiles)):
        if not os.path.exists(depthBakeOutPaths[i]):
            os.makedirs(depthBakeOutPaths[i])
        depthBakeOutPath = depthBakeOutPaths[i]
        viewAngleFile = viewAngleFiles[i]
        modelFile = modelFiles[i]
        inputUnit = inputUnits[i]
        RunDepthBake(depthBakeOutPath, viewAngleFile, modelFile, inputUnit)

def Analyze(fileMapping):
    for i in range(0,len(depthBakeOutPaths)):
        with open(viewAngleFiles[i], 'rb') as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            first = True
            for row in spamreader:
                if first:
                    first = False
                    continue
                filename = row[0].strip()
#                targetAz = float(row[1].strip())
#                MeasuredDepression = float(row[2].strip())
                if (fileMapping.has_key(filename.lower())):
                    dataPath = fileMapping[filename.lower()]
                    RDBO.ReadDepthBakeOutput(depthBakeOutPaths[i]+"depthbake_"+filename+"_plane0.raw", depthBakeOutPaths[i]+"depthbake_"+filename+"_plane1.raw", dataPath, depthBakeOutPaths[i]+filename)
                else:
                    print("Could not find "+filename.lower())

def filenameToPath(path):
    res = dict()    
    for f in listdir(path):
        fullFilePath = join(path,f)
        if isfile(fullFilePath):
            key = f.lower()
            res[key] = fullFilePath
        else:
            subres = filenameToPath(fullFilePath)
            for k in subres.keys():
                res[k] = subres[k]
    return res

fileMapping = filenameToPath(dataPath)
for i in range(len(viewAngleFiles)):
    RunDepthBake(depthBakeOutPaths[i], viewAngleFiles[i], modelFiles[i], inputUnits[0])

Analyze(fileMapping)

#