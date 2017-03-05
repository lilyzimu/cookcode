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

#trainlist  = '/home/hanli/caffe/examples/crf_alexnet_classify/test.txt'
trainlist = '/big/temp/hanli/data/moredata/zztestscene/test.txt'

with open (trainlist, "r") as myfile:
    inputdatastr=myfile.readlines()
totalTestpngNumber = len(inputdatastr)
print totalTestpngNumber

selectPlotOutTestpngNumber = totalTestpngNumber


#testpngpath = '/home/hanli/caffe/examples/crf3exposure/list.txt'
#plotoutpath = '/big/users/hanli/projects/data/classifyCRF/alexNet227/testscene/plotoutTestScene/'
TopNumber =  5;
plotcolors = ['r', 'k', 'b', 'm', 'g', 'y', 'c']

############ read PCA gt data ##############
nPCAcoms = 25;

wholepath = '/big/temp/hanli/data/classifyCRF/alexNet227/testscene/'
datapath = wholepath + 'raw/'
smallpath =  wholepath + 'small/'
pcapath = wholepath + 'pca/'
curvepath = wholepath + 'smallexposurecropcurvepngClassify/'

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

#f = os.popen('/home/hanli/caffe/build/tools/caffe test -model /home/hanli/caffe/examples/crf_alexnet_classify_withSeparateTrainAndTest/deployWithAccuracy.prototxt -weights /big/users/hanli/projects/data/classifyCRF/alexNet227/avoidOverfittingSeperateTrainandTest/snapshot10pm12042016_alexNetClassify/_iter_360000.caffemodel -iterations 20')
#now = f.read()
f = os.popen('/home/hanli/caffe/build/tools/caffe test -model /home/hanli/caffe/examples/crf6resnet/deploywithAccuracy.prototxt -weights /big/temp/hanli/data/moredata/datasnapshotcopy/snapshot/_iter_250000.caffemodel -iterations 20')
now = f.read()
