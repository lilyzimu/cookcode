import os, h5py
from os import listdir
import numpy as np
from datetime import datetime
from random import shuffle
import re

wholepath = '/big/temp/hanli/data/moredata/'
h5filespath = wholepath + '/pcacurve/'
#finaltrainpath = h5filespath 
finaltrainpath = wholepath

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
    f.write(h5filespath + trainh5files[i] + ' ' + str(curveIndex) + '\n');
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
    f.write(h5filespath + testh5files[i] + ' ' + str(curveIndex) + '\n');
f.close();
