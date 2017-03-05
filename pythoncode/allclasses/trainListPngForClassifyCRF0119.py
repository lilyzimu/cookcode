import os, h5py
from os import listdir
import numpy as np
from datetime import datetime
from random import shuffle
import re

wholepath = '/big/temp/hanli/data/moredata/zzcropdata/201classes50scenes/'
h5filespath = wholepath + '/croppcacurve/'
#finaltrainpath = h5filespath 
finaltrainpath = wholepath

#index starting from 0... 
#selectcurves = [4, 8, 9, 19, 34, 50, 53, 55, 56, 58, 61, 65, 70, 81, 93, 96, 103, 113, 124, 126, 154, 155, 159, 165, 167, 169, 183, 190, 193, 197, 200]

#selectscenes = [6, 9, 10, 12, 13, 14, 15, 17,18,27,49]
#selectcurves = [0, 1, 10, 12, 50, 169]
#selectcurves = [0, 1]

mean20txtpath = wholepath + 'mean20.txt'
mean30txtpath = wholepath + 'mean30.txt'
mean60txtpath = wholepath + 'mean60.txt'

def get_numbers_from_file(file_name):
    f = open(file_name, "r")
    strnumbers = f.read().split()
    f.close()
    return map(float, strnumbers)

with open(mean20txtpath) as f:
    meanlist = map(float, f)
mean20curves = np.asarray(meanlist).astype(int)
with open(mean30txtpath) as f:
    meanlist = map(float, f)
mean30curves = np.asarray(meanlist).astype(int)
with open(mean60txtpath) as f:
    meanlist = map(float, f)
mean60curves = np.asarray(meanlist).astype(int)


### remove shuffle, because I want to keep train/test data be from totally different image content
#shuffle(allcurvepngh5files)

# this is for training data set --> remove bad scenses. 
#allcurvepngh5files= [i for i in os.listdir(h5filespath) if i.endswith('.png') and 'scene9small' not in i and 'scene10small' not in i and 'scene25small' not in i and 'scene87small' not in i]

allcurvepngh5files= [i for i in os.listdir(h5filespath) if i.endswith('.png')]

totalnum = len(allcurvepngh5files)
print 'total num of images: ' 
print totalnum
#print len(allcurvepngh5files1)

#trainnum = int(totalnum/5) * 4
trainnum = totalnum


trainh5files = allcurvepngh5files
#trainh5files = allcurvepngh5files[0:trainnum]
#shuffle(trainh5files) 
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
    finalindex = curveIndex;
    f.write(h5filespath + trainh5files[i] + ' ' + str(finalindex) + '\n');
#    if( curveIndex in selectcurves):
#	finalindex = selectcurves.index(curveIndex);
#    	f.write(h5filespath + trainh5files[i] + ' ' + str(finalindex) + '\n');
f.close();

testh5files = allcurvepngh5files
#testh5files = allcurvepngh5files[trainnum:]
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
    finalindex = curveIndex;
    f.write(h5filespath + testh5files[i] + ' ' + str(finalindex) + '\n');
#    if( curveIndex in selectcurves) and ( sceneIndex in selectscenes):
#        finalindex = selectcurves.index(curveIndex);
#        f.write(h5filespath + trainh5files[i] + ' ' + str(finalindex) + '\n');

f.close();
