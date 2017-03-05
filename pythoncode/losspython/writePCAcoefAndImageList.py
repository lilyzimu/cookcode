import numpy as np
import matplotlib.pyplot as plt

import os, h5py
import re
from random import shuffle
from PIL import Image
from scipy import misc
from os import listdir


nPCAcoms = 25;

wholepath = '/big/temp/hanli/data/moredata/'
pcapath = wholepath + 'txt/'
curvepath = wholepath + 'pcacurve/'

meanfilename = pcapath + "meanOfBrightnessBYmatlabPCA.txt";
with open(meanfilename) as f:
    meanlist = map(float, f)
meanpcavector = np.asarray(meanlist)
meanpcavector = meanpcavector.reshape(1024, 1)
eigenfilename = pcapath+ "realeigenvector1024BYmatlabPCA25FromRAWtoJPG.txt";
with open(eigenfilename) as f:
    eigenlist = map(float, f)
eigenvector = np.asarray(eigenlist)
eigenvector = eigenvector.reshape(25, 1024)

curvefilename = pcapath+ "pcacoefsFor201curvesFromRAWtoJPG.txt";
with open(curvefilename) as f:
    curvelist = map(float, f)
curvepcacoefs = np.asarray(curvelist)
curvepcacoefs = curvepcacoefs.reshape(201, 25)


basisPCA = np.zeros((1024, 201))
basisPCA = basisPCA.astype('float32')

wholepath = '/big/temp/hanli/data/moredata/'
h5filespath = wholepath + 'pcacurve/'
finaltrainpath = wholepath


mean20txtpath = wholepath + 'mean20.txt'
mean30txtpath = wholepath + 'mean30.txt'
mean60txtpath = wholepath + 'mean60.txt'


with open(mean20txtpath) as f:
    meanlist = map(float, f)
mean20curves = np.asarray(meanlist).astype(int)
with open(mean30txtpath) as f:
    meanlist = map(float, f)
mean30curves = np.asarray(meanlist).astype(int)
with open(mean60txtpath) as f:
    meanlist = map(float, f)
mean60curves = np.asarray(meanlist).astype(int)


#index starting from 0... 

selectcurves = [1, 10, 13, 19, 22, 23, 25, 27, 29, 33, 34, 35, 37, 44, 49, 51, 52, 53, 54, 55, 56, 59, 60, 61, 62, 63, 64, 65, 67, 69, 70, 72, 75, 78, 81, 82, 84, 87, 93, 94, 100, 101, 102, 108, 109, 110, 117, 118, 119, 120, 121, 122, 123, 124, 126, 136, 137, 138, 139, 140, 142, 145, 146, 149, 150, 154, 155, 156, 157, 158, 159, 161, 163, 165, 166, 167, 168, 169, 170, 171, 172, 174, 180, 181, 182, 183, 189, 191, 192, 193]


allcurvepngh5files= [i for i in os.listdir(h5filespath) if i.endswith('.png')]

totalnum = len(allcurvepngh5files)
print 'total num of images: '
print totalnum

trainnum = totalnum
trainh5files = allcurvepngh5files
#trainh5files = allcurvepngh5files[0:trainnum]
shuffle(trainh5files)
trainlistfile = finaltrainpath + 'train.txt'
f = open(trainlistfile, "w")
for i in range(0, trainnum):
    imgnameNoExten = trainh5files[i].replace('.png', '')
    numbers= map(int, re.findall(r'\d+', imgnameNoExten))
    sceneIndex = numbers[0]
    curveIndex = numbers[1]
    winIndex = numbers[2]
    if (sceneIndex<50) or (curveIndex in mean20curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure3' in imgnameNoExten )or (curveIndex in mean30curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure7' in imgnameNoExten ):
        continue
    curveIndex = numbers[1]-1
#    finalindex = curveIndex;
#    f.write(h5filespath + trainh5files[i] + ' ' + str(finalindex) + '\n');
    if( curveIndex in selectcurves):
        finalindex = selectcurves.index(curveIndex);
	curve = curvepcacoefs[curveIndex,:].reshape(25,1);
        f.write(h5filespath + trainh5files[i] + ' ' );
	for coef in curve:
#		f.write(str(float(coef)) + ' ');	
		f.write(str('%.8f' % coef) + ' ');
	f.write("\n");
f.close();

testpngfilepath = '/big/temp/hanli/data/moredata/zzcropdata/201classes50scenes/croppcacurve/'
testpngfiles= [i for i in os.listdir(testpngfilepath) if i.endswith('.png')]

testh5files = testpngfiles
shuffle(testh5files)
testlistfile = finaltrainpath + 'test.txt'
f = open(testlistfile, "w")
for i in range(0, len(testh5files)):
    imgnameNoExten = testh5files[i].replace('.png', '')
    numbers= map(int, re.findall(r'\d+', imgnameNoExten))
    sceneIndex = numbers[0]
    curveIndex = numbers[1]
    winIndex = numbers[2]
#    if (sceneIndex>=50):
    if (sceneIndex>=50) or (curveIndex in mean20curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure3' in imgnameNoExten )or (curveIndex in mean30curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure5' in imgnameNoExten ):
        continue
    curveIndex = numbers[1]-1
#    finalindex = curveIndex;
#    f.write(h5filespath + testh5files[i] + ' ' + str(finalindex) + '\n');
    if( curveIndex in selectcurves) :
        finalindex = selectcurves.index(curveIndex);
	curve = curvepcacoefs[curveIndex,:].reshape(25,1);
        f.write(testpngfilepath+ testh5files[i] + ' ' );
        for coef in curve:
#                f.write(str(float(coef)) + ' ' );
		f.write(str('%.8f' % coef) + ' ');
	f.write("\n");
f.close();





