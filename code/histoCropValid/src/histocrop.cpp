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

void histocrop()
{
	/// make the RAW to JPGs & pfms
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
    string filepath = "/big/temp/hanli/data/moredata/pcacurve/";
    string wholeFilesPath = filepath ;
    string dstfilepath = "/big/temp/hanli/data/moredata/zzcropdata/201classes/croppcacurve/";
  
    int arrayscenes[11] = {6, 9,10, 12, 13, 14, 15, 17, 18,  27, 49};
    vector<int> selectScenes(arrayscenes, arrayscenes+11); 
 
    int imgStart = 1; // + 1  = 1. 
    int imgEnd = 50;
    int expolist[4] = { 20, 10, 7, 5};   
    int maxIter = 200;
    int totalSelectWin = 10;
    int selectNbinsfrombins = 220;
    int winsize = 227;
    
    string nefname = "scene";
    // make all JPGs according to gtLightingFactors
    #pragma omp parallel for
    for ( int i = imgStart; i <= imgEnd; i++ ) 
    {
     if(std::find(selectScenes.begin(), selectScenes.end(), i) != selectScenes.end())
     {
      #pragma omp parallel for 
      for ( int curveindex = 1; curveindex <= 201; curveindex++)
      {
	for( int expoindex = 0; expoindex < 4; expoindex++)
	{
		int exposure = expolist[expoindex];
		stringstream testi, curvestr, expostr;
        	testi <<i;
		curvestr << curveindex;
		expostr << exposure;
		string currentpngname = "scene" + testi.str() + "smallcurve" + curvestr.str() + "exposure" + expostr.str();   
		string pngext = wholeFilesPath + currentpngname + ".png";
//		std::ifstream myfile(pngext.c_str());
//		if (myfile.good())
		if( std::ifstream(pngext.c_str()))
		{
			cropValidWin(wholeFilesPath, dstfilepath, currentpngname, maxIter, totalSelectWin, selectNbinsfrombins, winsize); 
		}
	}
      }
     }
   }
    std::cout << "makeJpgs success: " << "currentDateTime()=" << currentDateTime() << std::endl;
    //////////////////////////////////////////////////////////

}

void cropValidWin( string filepath, string dstfilepath, string pngname, int maxIter, int totalSelectWin, int selectNbinsfrombins, int winsize)
{
    std::cout<< "load original images: " <<  pngname<< "   " << std::endl;
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
	
    string pngpath = filepath + pngname + ".png";
    cv::Mat src = imread( pngpath, 1 );
    int height = src.rows;
    int width = src.cols;
    std::cout << "height: " << height << " width: " << width << endl;

    int count = 0; 
    for(int i = 0; i < maxIter && count < totalSelectWin; i++)
    {
	int x = rand()%(width-winsize-1);
	int y = rand()%(height-winsize-1);	
	cv::Rect myROI(x, y, winsize, winsize);
	cv::Mat mycrop = src(myROI); 
	vector<Mat> bgr_planes;
	split( mycrop, bgr_planes );
	int histSize = 256;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	Mat b_hist, g_hist, r_hist;
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
//	for( int i = 1; i < histSize; i++ )
//	{
//		cout << " index " << i << " : " << b_hist.at<float>(i) << endl;
//	}
	int rnonzero = countNonZero(r_hist);
        int gnonzero = countNonZero(g_hist);
        int bnonzero = countNonZero(b_hist);
	if( rnonzero >= selectNbinsfrombins || gnonzero >= selectNbinsfrombins || bnonzero >= selectNbinsfrombins)
	{
		count++;
		stringstream cropstr;
		cropstr << count ;
		string cropimgpath = dstfilepath + pngname + "CropWin" + cropstr.str() + ".png";  		
		cv::imwrite(cropimgpath, mycrop);
	}
   }
}



