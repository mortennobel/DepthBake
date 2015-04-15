import ReadDepthBakeOutput
import sys
import os
import subprocess
import csv
from os import listdir
from os.path import isfile, join

argc = len(sys.argv)

depthBakePath = '/Users/mono/Library/Caches/appCode31/DerivedData/DepthBake-71380c64/Build/Products/Release/DepthBake'
depthBakeCWD = '/Users/mono/Programming/cpp/DepthBake/DepthBake/'
depthBakeOutPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/'
viewAngleDir = '/Users/mono/Programming/cpp/DepthBake/DepthBake/data'
viewAngleFile = '/Users/mono/Programming/cpp/DepthBake/DepthBake/data/btr70_transport_c71.csv'
modelFile = '/Users/mono/Programming/cpp/DepthBake/DepthBake/t-62.obj'

targetPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/t72_tank_132/depthbake_hb03333.0015_plane0.raw'
planePath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/t72_tank_132/depthbake_hb03333.0015_plane1.raw'
dataPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/Python/Data/MSTAR_PUBLIC_TARGETS_CHIPS_T72_BMP2_BTR70_SLICY/'
analysisOutput = '/Users/mono/Programming/cpp/DepthBake/DepthBake/test.png'
if argc > 1:
    targetPath = sys.argv[1]
if argc > 2:
    planePath = sys.argv[2]
if argc > 3:
    planePath = sys.argv[3]


viewAngleFiles = [#"/Users/mono/Programming/cpp/DepthBake/DepthBake/data/bmp2_tank_9563.csv",
              #"/Users/mono/Programming/cpp/DepthBake/DepthBake/data/bmp2_tank_9566.csv",
              #"/Users/mono/Programming/cpp/DepthBake/DepthBake/data/bmp2_tank_c21.csv",
              # "/Users/mono/Programming/cpp/DepthBake/DepthBake/data/btr70_transport_c71.csv",
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/data/t72_tank_132.csv", #version 1
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/data/t72_tank_812.csv", #version 1
              #"/Users/mono/Programming/cpp/DepthBake/DepthBake/data/t72_tank_s7.csv" #version 2
              ]

modelFiles = [#"/Users/mono/Programming/cpp/DepthBake/DepthBake/models/BMP-2.obj",
              # "/Users/mono/Programming/cpp/DepthBake/DepthBake/models/BTR70.obj",
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/models/T72_Version1.obj",
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/models/T72_Version1.obj",
              #"/Users/mono/Programming/cpp/DepthBake/DepthBake/models/T72_Version2.obj",
              ]

depthBakeOutPaths = [#"/Users/mono/Programming/cpp/DepthBake/DepthBake/models/BMP-2.obj",
              # "/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/btr70_transport_c71/",
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/t72_tank_132/",
              "/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/t72_tank_812/",
              #"/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/t72_tank_s7/",
              ]

inputUnits = ["inch",
              "inch",
              #"inch",
              #"mm",
              ]

def AllRunDepthBake():
    for i in range(0,len(viewAngleFiles)):
        if not os.path.exists(depthBakeOutPaths[i]):
            os.makedirs(depthBakeOutPaths[i])
        depthBakeOutPath = depthBakeOutPaths[i]
        viewAngleFile = viewAngleFiles[i]
        modelFile = modelFiles[i]
        inputUnit = inputUnits[i]
        RunDepthBake(depthBakeOutPath, viewAngleFile, modelFile, inputUnit)

def RunDepthBake(depthBakeOutPath, viewAngleFile, modelFile, inputUnit):
    # FNULL = open(os.devnull, 'w')    #use this if you want to suppress output to stdout from the subprocess using stdout=FNULL, stderr=FNULL,
    args = depthBakePath + " -inputUnit "+inputUnit+" -outputPath " + depthBakeOutPath + " -viewAngleFile " + viewAngleFile + ' '+modelFile
    print(args)
    subprocess.call(args, shell=True, cwd=depthBakeCWD)

def Analyze(fileMapping):
    for i in range(0,len(depthBakeOutPaths)):
        with open(viewAngleFile, 'rb') as csvfile:
            spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
            first = True
            for row in spamreader:
                if first:
                    first = False
                    continue
                filename = row[0].strip()
                targetAz = float(row[1].strip())
                MeasuredDepression = float(row[2].strip())
                if (fileMapping.has_key(filename.lower())):
                    dataPath = fileMapping[filename.lower()]
                    ReadDepthBakeOutput.ReadDepthBakeOutput(depthBakeOutPaths[i]+"depthbake_"+filename.lower()+"_plane0.raw", depthBakeOutPaths[i]+"depthbake_"+filename.lower()+"_plane1.raw", dataPath, depthBakeOutPaths[i]+filename)
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

def svnFiles():
    res = [ f for f in listdir(viewAngleDir) if isfile(join(viewAngleDir,f))]
    for i in range(0,len(res)):
        res[i] = join(viewAngleDir,res[i])
    return res

fileMapping = filenameToPath(dataPath)

# modify filemapping
#for k in fileMapping.keys():
#    modKey = k[:len(k)-3] +"0"+ k[len(k)-3:]
#    fileMapping[modKey] = fileMapping[k]

#print("File mappings")
#for k in fileMapping.keys():
#        print(k + "\t"+fileMapping[k])
#print("File mappings end")

#ReadDepthBakeOutput.ReadDepthBakeOutput(targetPath, planePath, fileMapping["HB03333.0015"], targetPath+".png")

AllRunDepthBake()
#RunDepthBake()
Analyze(fileMapping)

#