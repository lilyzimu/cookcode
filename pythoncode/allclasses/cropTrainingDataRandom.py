import os 
from os import listdir
import cv2
import numpy as np
import random
from matplotlib import pyplot as plt
from PIL import Image
from datetime import datetime
from random import shuffle
import re

wholepath = '/big/temp/hanli/data/moredata/'
datapath = '/big/temp/hanli/data/moredata/pcacurve/'
cropdatatxtpath = '/big/temp/hanli/data/moredata/zzcropdata/201classestrainData/croppcacurvetxt/'
cropdatapath = '/big/temp/hanli/data/moredata/zzcropdata/201classestrainData/croppcacurve/'

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

totalSelectWin = 5;
totalBinsNum = 256;
selectNbinsfrombins = 220;
maxIter = 100;
L = 227;

#selectcurves = [4, 8, 9, 19, 34, 50, 53, 55, 56, 58, 61, 65, 70, 81, 93, 96, 103, 113, 124, 126, 154, 155, 159, 165, 167, 169, 183, 190, 193, 197, 200]


h5filespath = datapath;
allcurvepngh5files= [i for i in os.listdir(h5filespath) if i.endswith('.png')]
totalnum = len(allcurvepngh5files)
trainnum = totalnum

shuffle(allcurvepngh5files)

for i in range(0, trainnum):
 imgnameNoExten = allcurvepngh5files[i].replace('.png', '')
 numbers= map(int, re.findall(r'\d+', imgnameNoExten))
 sceneIndex = numbers[0]
 curveIndex = numbers[1]
 
 if (sceneIndex<50) or (curveIndex in mean20curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean20curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure3' in imgnameNoExten )or (curveIndex in mean30curves and 'exposure5' in imgnameNoExten ) or (curveIndex in mean30curves and 'exposure7' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure3' in imgnameNoExten ) or (curveIndex in mean60curves and 'exposure5' in imgnameNoExten ):
    continue
 
 curveIndex = numbers[1]-1
 if ( sceneIndex >=50 ):
# if( curveIndex in selectcurves):
    print 'begin scene' + str(imgnameNoExten);
    print str(datetime.now())
    imgname = imgnameNoExten;
    imgpath = h5filespath + allcurvepngh5files[i];

#for imgindex in range(1,10):
#  for curveindex in range(1,3):
#    print 'begin scene' + str(imgindex) + 'small';
#    print str(datetime.now())
#    imgname = 'scene' + str(imgindex) + 'smallcurve' + str(curveindex) + 'exposure10'

#    imgpath = datapath + imgname + '.png';

    img = Image.open(imgpath)
    
    filename = cropdatatxtpath + imgname + 'selectWin.txt'
    f = open(filename, 'w')

    width = img.size[0]
    height = img.size[1]    
    img.close()
    
    goodWin = 0;
    winlist = [];
    for i in range(0, maxIter):

	img = Image.open(imgpath)                
        y = random.randint(0, height-L)
        x = random.randint(0, width-L)
        cropimg = img.crop((x, y, x+L, y+L))
        img.close();
 
        cropdata = list(cropimg.getdata())
        cropdata = np.asarray(cropdata)
        cropdata = np.hstack(cropdata)
  
        if totalBinsNum == 32:
            cropdata = cropdata * 1.0/8.0;
        bins = np.arange(0,totalBinsNum,1)
        hist, bin_edges = np.histogram(cropdata, bins=bins, density=True);

	avgpixel = np.mean(cropdata)
	blackpixelnum = (cropdata< 20).sum()
	blackrange = L * L * 3 * 0.25; 

        nonzeroHist = np.count_nonzero(hist)
        if nonzeroHist >= selectNbinsfrombins and (avgpixel >=50) and (blackpixelnum < blackrange):
            ## comment following code, becasue it will casue the loop become super time-comsumping as index increase...  after comment,  this code can run super fast, only few seconds for one image for 100 iterations. 
            #cropimgpath = cropdatapath + imgname + 'CropWin' + str(goodWin) + '.png';
            #cropimg.save(cropimgpath);

            ##plt.hist(cropdata, bins=bins, color='green')
            ##plt.xlim(0,32)
            ##plt.title('histogram of all rgb values')
            ##histpath = cropdatapath + imgname + 'CropWin' + str(goodWin) + 'hist32.png';
            ##plt.savefig(histpath);
            
            #plt.hist(cropdata, bins=np.arange(0,256,1), color='green')
            #plt.xlim(0,256)
            #plt.title('histogram of all rgb values')
            #histpath = cropdatapath + imgname + 'CropWin' + str(goodWin) + 'hist256.png';
            #plt.savefig(histpath);

	    print 'avgpixel: ' 
	    print avgpixel;
            winlist.append( (x,y) );
            goodWin = goodWin + 1;
	
	    print np.min(cropdata)
            print np.max(cropdata)
		
	    cropimgpath = cropdatapath + imgname + 'CropWin' + str(goodWin) + '.png';
            cropimg.save(cropimgpath);	
		
	    img = Image.open(cropimgpath)
	    cropdata = list(img.getdata())
            cropdata = np.asarray(cropdata)
            cropdata = np.hstack(cropdata)
	    print "compare: " 
	    print np.min(cropdata)
            print np.max(cropdata)

        if goodWin == totalSelectWin :
            break
        
    
    print 'imgindex:' + str(imgnameNoExten) +  ' list: '
    print winlist;
    print len(winlist);

#f.write('scene' + str(imgindex) + ' ' + str(len(winlist)) + '\n')
    f.write('\n'.join('%s %s' %x for x in winlist))
    f.write('\n');
    f.close();

#print 'found' + str(goodWin)
print 'done'

