import ReadDepthBakeOutput
import sys
import os
import subprocess
import csv

argc = len(sys.argv)

depthBakePath = '/Users/mono/Library/Caches/appCode31/DerivedData/DepthBake-71380c64/Build/Products/Release/DepthBake'
depthBakeCWD = '/Users/mono/Programming/cpp/DepthBake/DepthBake/'
depthBakeOutPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/'
viewAngleFile = '/Users/mono/Programming/cpp/DepthBake/DepthBake/t62.csv'
modelFile = '/Users/mono/Programming/cpp/DepthBake/DepthBake/t-62.obj'

targetPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/depthbake_hb14931.0016_plane0.raw'
planePath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/baked-data/depthbake_hb14931.0016_plane1.raw'
dataPath = '/Users/mono/Programming/cpp/DepthBake/DepthBake/Python/Data/MSTAR/MSTAR_PUBLIC_MIXED_TARGETS_CD1/15_DEG/COL2/SCENE1/T62/HB14931.016'
if argc > 1:
    targetPath = sys.argv[1]
if argc > 2:
    planePath = sys.argv[2]
if argc > 3:
    planePath = sys.argv[3]

def RunDepthBake():
    # FNULL = open(os.devnull, 'w')    #use this if you want to suppress output to stdout from the subprocess using stdout=FNULL, stderr=FNULL,
    args = depthBakePath + " -inputUnit mm -outputPath " + depthBakeOutPath + " -viewAngleFile " + viewAngleFile + ' '+modelFile
    subprocess.call(args, shell=True, cwd=depthBakeCWD)

def Analyze():
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

RunDepthBake()
#Analyze()

#ReadDepthBakeOutput.ReadDepthBakeOutput(targetPath, planePath, dataPath)