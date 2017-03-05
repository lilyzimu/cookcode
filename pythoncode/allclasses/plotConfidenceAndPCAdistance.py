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
from numpy import linalg as LA
from scipy.spatial import distance


#trainlist  = '/home/hanli/caffe/examples/crf_alexnet_classify/test.txt'
trainlist = '/big/temp/hanli/data/moredata/zzcropdata/201classes50scenes/test.txt'

with open (trainlist, "r") as myfile:
    inputdatastr=myfile.readlines()
totalTestpngNumber = len(inputdatastr)
print totalTestpngNumber

selectPlotOutTestpngNumber = totalTestpngNumber
#selectPlotOutTestpngNumber = 1

#testpngpath = '/big/temp/hanli/data/moredata/zzcropdata/201classes50scenes/test.txt'
testpngpath = '/home/hanli/caffe/examples/crf6resnet/list.txt'
plotoutpath = '/big/temp/hanli/data/moredata/zzplotout/pcadistance201classes50scenes/plotoutTestScene/'
top1curveWithImagepath = '/big/temp/hanli/data/moredata/zzplotout/pcadistance201classes50scenes/curveWithTestImagePlotOut/top1/'
top5curveWithImagepath = '/big/temp/hanli/data/moredata/zzplotout/pcadistance201classes50scenes/curveWithTestImagePlotOut/top5/'
wrongcurveWithImagepath = '/big/temp/hanli/data/moredata/zzplotout/pcadistance201classes50scenes/curveWithTestImagePlotOut/wrong/'

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

curvepngpath = wholepath + 'zzcropdata/201classes50scenes/croppcacurve/'

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

for index in range(0, selectPlotOutTestpngNumber):
    print 'begin index:' 
    print index
    ##### get ground truth curve #####
    sep = ' '
    cropcurvename =  (str(inputdatastr[index]).split(sep)[0]).split('croppcacurve/')[1]
#    cropcurvename = 'scene1smallcurve1CropWin0.png'
    imgnameNoExten = cropcurvename.replace('.png', '')
    print 'imgnameNoExten'
    numbers= map(int, re.findall(r'\d+', imgnameNoExten))
    sceneIndex = numbers[0]
    curveIndex = numbers[1]-1
    winIndex = numbers[2]
    curve = curvepcacoefs[curveIndex,:].reshape(25,1);
    
    ###### estimate CRF ######
    pngandlabel = str(inputdatastr[index])
    print pngandlabel
    text_file = open(testpngpath, "w")
    text_file.write(pngandlabel)
    text_file.close()
    
    f = os.popen('/home/hanli/caffe/build/tools/caffe test -model /home/hanli/caffe/examples/crf6resnet/deploy.prototxt -weights /big/temp/hanli/data/moredata/datasnapshotcopy/snapshot0117resnet_accuracy074best/_iter_30000.caffemodel -iterations 1') 
    now = f.read()
    
    top_inds = np.zeros(TopNumber)
    with h5py.File('/big/temp/hanli/data/moredata/datasnapshotcopy/snapshot/output.h5','r') as hf:
        #print('List of items in the base directory:', hf.items())
        #print('List of arrays in this file: \n', hf.keys())
        prob = hf.get('label')
        np_prob = np.array(prob).reshape(totalClassNum)
        top_inds = np_prob.argsort()[::-1][:TopNumber]

    print top_inds
    
    ###### plot out estimated CRF & gt CRF #####
    fig = plt.figure()
    figname = plotoutpath + cropcurvename
    #print out the gt curve & est curve
    rawdata = np.zeros((1024,1))
    gtpngdata = np.zeros((1024,1))
    estpngdata = np.zeros((1024,TopNumber))
    
    # obtain the output pca coefficients
    output_curveIndex = np.zeros(TopNumber)
    output_pcacoef = np.zeros((25, TopNumber))
    for estTopN in range(0,TopNumber):
#	finalindex = selectcurves[abs(top_inds[estTopN])];
#	output_curveIndex[estTopN] = finalindex;	
        output_curveIndex[estTopN] = abs(top_inds[estTopN])  ##### begin from 0??? 

        output_pcacoef[:,estTopN] = curvepcacoefs[output_curveIndex[estTopN],:].reshape(25);
       
    pcacoefdis = np.zeros((TopNumber, 1))
    pcacurvedis = np.zeros((TopNumber,1)) 
    for i in range (0, 1024):
        rawdata[i] = i/1024.0
        for j in range(0,25):
            gtpngdata[i] = gtpngdata[i] + curve[j] * eigenvector[j, i]
        gtpngdata[i] = gtpngdata[i] + meanpcavector[i]
        
        for estTopN in range(0,TopNumber):
            for j in range(0,25):
                estpngdata[i, estTopN] = estpngdata[i, estTopN] + output_pcacoef[j, estTopN] * eigenvector[j, i]
            estpngdata[i, estTopN] = estpngdata[i, estTopN] + meanpcavector[i]

    print '25 coefs: ';
    print curve;
    print output_pcacoef[:, 1];
    print gtpngdata; 
    print estpngdata[:,1];

    print eigenvector;
 
    print 'plot out minus result';
    test = curve.reshape(25) - output_pcacoef[:, 1].reshape(25);
    print test;
    print LA.norm(test);
    print 'distance ' 
    print distance.euclidean(curve.reshape(25), output_pcacoef[:, 1].reshape(25)); 
   
    print 'print gtpngdata'; 
    test1 = gtpngdata.reshape(1024) - estpngdata[:,1].reshape(1024);
    print test1;
    print LA.norm(test1);
    print 'distance' 
    print distance.euclidean(gtpngdata.reshape(1024), estpngdata[:,1].reshape(1024))
 
    for estTopN in range(0,TopNumber):
	pcacoefdis[estTopN] = LA.norm(curve.reshape(25)-output_pcacoef[:, estTopN].reshape(25))
	pcacurvedis[estTopN] = LA.norm(gtpngdata.reshape(1024) - estpngdata[:,estTopN].reshape(1024))  
	 
    
    plt.plot(rawdata, gtpngdata, color='pink', linewidth=2.5, linestyle="-", label="gt curve: " + str(curveIndex) )
    for estTopN in range(0,TopNumber):
#        plt.plot(rawdata, estpngdata[:,estTopN], color=plotcolors[estTopN], linewidth=1.5, linestyle="--", label='estTop' + str(estTopN) + 'curve: ' + str(int((output_curveIndex[estTopN]))) + ' prob: ' + str(float(np_prob[top_inds[estTopN]])) + ' pcacoef: ' + str(float(pcacoefdis[estTopN])) + ' pcacurve: ' + str(float(pcacurvedis[estTopN])) )
#	plt.plot(rawdata, estpngdata[:,estTopN], color=plotcolors[estTopN], linewidth=1.5, linestyle="--", label= 'prob: ' + str("%.3f" % (np_prob[top_inds[estTopN]])) + ' coef: ' + str("%.3f" %(pcacoefdis[estTopN])) + ' curve: ' + str("%.3f" %(pcacurvedis[estTopN])) + ' i' + str(estTopN) + ': ' + str(int((output_curveIndex[estTopN]))))
        plt.plot(rawdata, estpngdata[:,estTopN], color=plotcolors[estTopN], linewidth=1.5, linestyle="--", label= 'prob: ' + str("%.3f" % (np_prob[top_inds[estTopN]])) + ' coef: ' + str("%.3f" %(pcacoefdis[estTopN])) + ' i' + str(estTopN) + ': ' + str(int((output_curveIndex[estTopN]))))

    plt.legend(loc='lower right', frameon=False)

    fig.savefig(figname, dpi=fig.dpi)

    img1path = curvepngpath + cropcurvename;
    img2path = plotoutpath + cropcurvename;
    print img1path;
    print img2path;
    img1 = cv2.imread(img1path)
    img2 = cv2.imread(img2path)

    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]
    vis = np.zeros( (max(h1,h2), w1+w2, 3) )
    vis[:h1, :w1, : ] = img1
    vis[:h2, w1:w1+w2, :] = img2
    #vis = np.concatenate((img1, img2), axis = 1)

    if curveIndex == int((output_curveIndex[0])):
	curveWithImagepath = top1curveWithImagepath;
    elif curveIndex in output_curveIndex:
	curveWithImagepath = top5curveWithImagepath;
    else: 
	curveWithImagepath = wrongcurveWithImagepath; 
    outputpath = curveWithImagepath + cropcurvename;
    cv2.imwrite(outputpath, vis);

