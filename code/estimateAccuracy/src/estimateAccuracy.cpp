/**
 * @author Han Li
 */

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>


//add pieter lab
#include "image.h"
#include "imageIO.h"
#include "color.h"

#include "displaytime.h"
#include "readfile.h"

//add opencv lab
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void cropValidWin( string filepath, string dstfilepath, string pngname, int maxIter, int totalSelectWin, int selectNbinsfrombins, int winsize);

void estimateAccuracy()
{
	/// make the RAW to JPGs & pfms
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
    string filepath= "/big/temp/hanli/data/moredata/zzcropdata/201classes50scenes/croppcacurve/";
    string wholeFilesPath = filepath ;
    string resnetpath = "/home/hanli/caffe/examples/crf10lessclass/";
    string hdf5path = "/big/temp/hanli/data/moredata/zzcropdata/90classes/cplusplus/hdf5/";

    int arrayscenes[11] = {6, 9,10, 12, 13, 14, 15, 17, 18,  27, 49};
    vector<int> selectScenes(arrayscenes, arrayscenes+11); 
 
    int imgStart = 1; // + 1  = 1. 
    int imgEnd = 50;
    int expolist[4] = { 20, 10, 7, 5};   
    int maxIter = 200;
    int totalSelectWin = 10;
    int selectNbinsfrombins = 220;
    int winsize = 227;
 
    cout << " begin process : " << endl;   
    // make all JPGs according to gtLightingFactors
    #pragma omp parallel for
    for ( int i = imgStart; i <= imgEnd; i++ ) 
    {
     if(std::find(selectScenes.begin(), selectScenes.end(), i) != selectScenes.end())
     {
	cout << "scene " << i << endl;
      #pragma omp parallel for 
      for ( int curveindex = 1; curveindex <= 201; curveindex++)
      {
	for( int expoindex = 0; expoindex < 4; expoindex++)
	{
		int label  = curveindex-1;
		int exposure = expolist[expoindex];
		stringstream testi, curvestr, expostr;
        	testi <<i;
		curvestr << curveindex;
		expostr << exposure;
		string currentpngname = "scene" + testi.str() + "smallcurve" + curvestr.str() + "exposure" + expostr.str();   
		for( int cropindex = 1 ; cropindex <= 10; cropindex++)
		{
			stringstream cropstr;
			cropstr << cropindex;
			string pngext = wholeFilesPath + currentpngname + "CropWin" + cropstr.str() + ".png";
	//		std::ifstream myfile(pngext.c_str());
	//		if (myfile.good())
			if( std::ifstream(pngext.c_str()))
			{
				cout << pngext << endl;
				std::ofstream listcplustxt;
				string listcplustxtpathstr = resnetpath + "listcplus.txt";
				char const* listcplustxtpath = listcplustxtpathstr.c_str();
				listcplustxt.open(listcplustxtpath);
				stringstream labelstr;
				labelstr << label;
				listcplustxt << pngext << " " << labelstr.str();
				listcplustxt.close();  
				system("/home/hanli/caffe/build/tools/caffe test -model /home/hanli/caffe/examples/crf10lessclass/deploycplus.prototxt -weights /big/temp/hanli/data/moredata/datasnapshotcopy/snapshot0224_resnet_90classes/_iter_250000.caffemodel -iterations 1");				
				cout << "input label: " << label << endl;
				string oldhdf5pathstr = "/big/temp/hanli/data/moredata/zzcropdata/90classes/cplusplus/outputcplus.h5";
				char const* oldhdf5path = oldhdf5pathstr.c_str();
				string newhdf5pathstr = hdf5path + currentpngname + "CropWin" + cropstr.str() + ".h5";
				char const* newhdf5path = newhdf5pathstr.c_str();
				int result= rename( oldhdf5path, newhdf5path);  
			}		

	
		}
	}
      }
     }
   }
    std::cout << "makeJpgs success: " << "currentDateTime()=" << currentDateTime() << std::endl;
    //////////////////////////////////////////////////////////

}

