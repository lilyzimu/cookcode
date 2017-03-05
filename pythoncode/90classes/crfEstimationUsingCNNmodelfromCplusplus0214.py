''' using : 
hanli@cook:~/caffe> ./build/tools/caffe test 
-model examples/crf_alexnet_classify/deploy.prototxt 
-weights /big/users/hanli/projects/data/classifyCRF/alexNet227/snapshotlr1e06/_iter_90000.caffemodel 
-iterations 1

to directly generate prob. 

then: 
hanli@cook:/big/users/hanli/projects/data/classifyCRF/alexNet227/snapshot> ll
total 608
-rw-r--r-- 1 hanli users 621296 Nov 27 14:54 output.h5

''' 
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
#import matplotlib.pyplot as plt
#%matplotlib inline
import re
import h5py, os 
import cv2
from collections import Counter

#trainlist  = '/home/hanli/caffe/examples/crf_alexnet_classify/test.txt'
trainlist = '/home/hanli/caffe/examples/crf10lessclass/test.txt'

with open (trainlist, "r") as myfile:
    inputdatastr=myfile.readlines()
totalTestpngNumber = len(inputdatastr)
print totalTestpngNumber

selectPlotOutTestpngNumber = totalTestpngNumber
#selectPlotOutTestpngNumber = 1

testpngpath = '/home/hanli/caffe/examples/crf6resnet/list.txt'
plotoutpath = '/big/temp/hanli/data/moredata/zzplotout/201classes/plotoutTestScene/'
curveWithImagepath = '/big/temp/hanli/data/moredata/zzplotout/201classes/curveWithTestImagePlotOut/'

TopNumber =  5;
totalClassNum = 201;
plotcolors = ['r', 'k', 'b', 'm', 'g', 'y', 'c']
#selectcurves = [4, 8, 9, 19, 34, 50, 53, 55, 56, 58, 61, 65, 70, 81, 93, 96, 103, 113, 124, 126, 154, 155, 159, 165, 167, 169, 183, 190, 193, 197, 200]

############ read PCA gt data ##############
nPCAcoms = 25;

wholepath = '/big/temp/hanli/data/moredata/'
#datapath = wholepath + 'raw/'
#smallpath =  wholepath + 'small/'
pcapath = wholepath + 'txt/'
#curvepath = wholepath + 'smallcurve/'

curvepngpath = wholepath + 'zzcropdata/201classes/croppcacurve/'

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

###############################################
wholepath = '/big/temp/hanli/data/moredata/zzcropdata/90classes/cplusplus/'
pngpath = wholepath + 'hdf5/';
accuracypath = wholepath + 'accuracy.txt';
esttxtpath = wholepath + 'est.txt';
selectscenes = [6, 9, 10, 12, 13, 14, 15, 17,18,27,49];
expolist = [20, 10, 7, 5]

selectcurves = [1, 10, 13, 19, 22, 23, 25, 27, 29, 33, 34, 35, 37, 44, 49, 51, 52, 53, 54, 55, 56, 59, 60, 61, 62, 63, 64, 65, 67, 69, 70, 72, 75, 78, 81, 82, 84, 87, 93, 94, 100, 101, 102, 108, 109, 110, 117, 118, 119, 120, 121, 122, 123, 124, 126, 136, 137, 138, 139, 140, 142, 145, 146, 149, 150, 154, 155, 156, 157, 158, 159, 161, 163, 165, 166, 167, 168, 169, 170, 171, 172, 174, 180, 181, 182, 183, 189, 191, 192, 193]


correct1 = 0;
correct5 = 0;
totalpngnum = 0;
curaccuracy1 = 0;
curaccuracy5 = 0;
for sceneIndex in selectscenes:
  for expo in expolist:
    for curveIndex in range(0, 201):
	imgname = 'scene' + str(sceneIndex) + 'smallcurve' + str(curveIndex+1) + 'exposure' + str(expo);
	print imgname;
	cropwins = [i for i in os.listdir(pngpath) if imgname in i]
	print cropwins
	curve = curvepcacoefs[curveIndex,:].reshape(25,1);
	top1 = [];
	top5 = [];
	for cropcurvename in cropwins:
		imgnameNoExten = cropcurvename.replace('.h5', '')
		pngandlabel = curvepngpath + cropcurvename + ' ' + str(curveIndex);
		hdf5path = pngpath + cropcurvename;
		with h5py.File(hdf5path, 'r') as hf: 
			prob = hf.get('label')
			np_prob = np.array(prob).reshape(90) 
   
                top_inds = np.zeros(TopNumber) 
        	top_inds = np_prob.argsort()[::-1][:TopNumber]
   		top5.extend(top_inds);
		top1.append(top_inds[0]);			
	if top1 and (curveIndex in selectcurves):
		print top1;
		print top5;
		counttop1 = Counter(top1); 
		counttop5 = Counter(top5);
		esttop1 = counttop1.most_common()[0];
		esttop5 = counttop5.most_common(2);
		print esttop1;
		print esttop5;
		print esttop1[0];
		print esttop5[0][0];
#		esttop5curves = [esttop5[0][0], esttop5[1][0], esttop5[2][0], esttop5[3][0], esttop5[4][0]]
		esttop5curves = [esttop5[0][0], esttop5[1][0]]
		
		finalindex = selectcurves.index(curveIndex);
		if (finalindex==esttop1[0]):
			correct1 = correct1 + 1;
		if finalindex in esttop5curves:
			correct5 = correct5 + 1;
		totalpngnum = totalpngnum + 1;
		curaccuracy1 = correct1/float(totalpngnum);
		curaccuracy5 = correct5/float(totalpngnum);
		accuracytxtfile = open(accuracypath, "w")
		accuracytxtfile.write(str(curaccuracy1)+'\n');
		accuracytxtfile.write(str(curaccuracy5)+'\n');
		accuracytxtfile.close();
#accuracytxtfile.close();

		
   			 
