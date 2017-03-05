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

trainlist = '/home/hanli/caffe/examples/crf10lessclass/train.txt'
finallist = '/home/hanli/caffe/examples/crf12crfloss/train.txt'
f = open(finallist, "w")


with open (trainlist, "r") as myfile:
    inputdatastr=myfile.readlines()
totalTestpngNumber = len(inputdatastr)
print totalTestpngNumber

selectPlotOutTestpngNumber = totalTestpngNumber


for index in range(0, selectPlotOutTestpngNumber):
#    print 'begin index:'
#    print index
##### get ground truth curve #####
    sep = ' '
    cropcurvename =  (str(inputdatastr[index]).split(sep)[0])
    f.write(cropcurvename+'\n');
f.close(); 
