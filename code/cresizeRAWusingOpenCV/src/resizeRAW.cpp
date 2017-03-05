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

// add opencv to resize
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
//using namespace cv;


void resizeprogram( string filepath, string nef, string nefformat, int imgIndex, int scale );


//int main( int argc, char** argv )
void resizeRAW()
{
	/// make the RAW to JPGs & pfms
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
    string filepath = "/big/temp/hanli/data/moredata/zztestscene/";
    string wholeFilesPath = filepath ;

    int scale = 8;
    
    int imgStart = 6;  
    int imgEnd = 6;
    
    string nefname = "scene";
    string nefformat = ".NEF";   
    // make all JPGs according to gtLightingFactors
    #pragma omp parallel for 
    for ( int i = imgStart; i <= imgEnd; i++ )
    {
	stringstream testi;
	testi << i;
	string currentnefname = nefname + testi.str();
	resizeprogram(wholeFilesPath, currentnefname, nefformat, i, scale);
    }
    std::cout << "makeJpgs success: " << "currentDateTime()=" << currentDateTime() << std::endl;
    //////////////////////////////////////////////////////////

}

void resizeprogram( string filepath, string nef, string nefformat, int imgIndex, int scale )
{
	std::cout<< "load original images: " <<  imgIndex << "   " << std::endl;

	image img_0;
	string rawpath = filepath + "raw/";
	string filepathimg0 = rawpath + nef + nefformat;
	io::importImage(filepathimg0, img_0);
//	std::cout<< "image0 width: " << img_0.width() << "  image0 height: " << img_0.height() << std::endl;
	int rescalewidth = img_0.width()/scale;
	int rescaleheight = img_0.height()/scale; 
	image poly_0(rescalewidth,  rescaleheight);

	int oriwidth = img_0.width();
	int oriheight = img_0.height();
	float *oriimgdata0 = new float[oriwidth*oriheight];
        float *oriimgdata1 = new float[oriwidth*oriheight];
        float *oriimgdata2 = new float[oriwidth*oriheight];

	std::cout << " begin writing oriimgdata " << std::endl; 
	#pragma omp parallel for 
	for(int x = 0; x < oriwidth; x++ ) 
		#pragma omp parallel for 
		for( int y = 0; y < oriheight; y++)
		{
			int index = x * oriheight + y;
			//cout << " x : " << x << "  y: " << y << endl;
			oriimgdata0[index] = img_0(x,y).r;
			oriimgdata1[index] = img_0(x,y).g;
                        oriimgdata2[index] = img_0(x,y).b;
		}

	cv::Mat orimat0 = cv::Mat(oriwidth, oriheight, CV_32FC1, oriimgdata0);
        cv::Mat orimat1 = cv::Mat(oriwidth, oriheight, CV_32FC1, oriimgdata1);
        cv::Mat orimat2 = cv::Mat(oriwidth, oriheight, CV_32FC1, oriimgdata2);
/*
	image oriimg(oriwidth, oriheight);
        for(int x = 0; x < oriwidth; x++ )
                for(int y = 0; y < oriheight; y++ )
                {
                        oriimg(x,y).r = orimat0.at<float>(x,y);
                        oriimg(x,y).g = orimat1.at<float>(x,y);
                        oriimg(x,y).b = orimat2.at<float>(x,y);

                }
        string pathoriimg = filepath + "/opencvtest/small/" + nef + "big" + ".png";
        io::exportImage(pathoriimg, oriimg);
*/
	cout << " done orimat " << endl;
	int dstsize[3] = {rescalewidth, rescaleheight, 3};
//	cv::Mat dst = cv::Mat(3, dstsize, CV_32FC3, cv::Scalar::all(0));
	cv::Mat dst0, dst1, dst2;
	cout << " orimat.cols " << orimat0.cols << "orimat.rows" << orimat0.rows << endl;

	cv::resize(orimat0, dst0, cv::Size(orimat0.cols / scale, orimat0.rows / scale), cv::INTER_AREA);
        cv::resize(orimat1, dst1, cv::Size(orimat1.cols / scale, orimat1.rows / scale), cv::INTER_AREA);
        cv::resize(orimat2, dst2, cv::Size(orimat2.cols / scale, orimat2.rows / scale), cv::INTER_AREA);


//	cv::resize(orimat0, dst0, cv::Size(orimat0.cols / 2, orimat0.rows / 2);
//	cv::resize(orimat1, dst1, cv::Size(orimat1.cols / 2, orimat1.rows / 2);
//	cv::resize(orimat2, dst2, cv::Size(orimat2.cols / 2, orimat2.rows / 2);



	cout << " done resize " << endl;
//	dst.convertTo(dst, CV_16UC3);
	image smallimg(rescalewidth,  rescaleheight);
	for(int x = 0; x < rescalewidth; x++ )
		for(int y = 0; y < rescaleheight; y++ )
		{
			smallimg(x,y).r = dst0.at<float>(x,y);
                        smallimg(x,y).g = dst1.at<float>(x,y);
                        smallimg(x,y).b = dst2.at<float>(x,y);

		}

	cout << " done dst " << endl;
        delete [] oriimgdata0;
        delete [] oriimgdata1;
        delete [] oriimgdata2;
	
	#pragma omp parallel for 
	for(int y = 0; y < rescaleheight; y++ )
		#pragma omp parallel for 
		for(int x = 0; x < rescalewidth; x++ )
		{
			double cr = 0.0, cg = 0.0, cb = 0.0;
			for( int iy = 0; iy < scale; iy++)
				for( int ix = 0; ix < scale; ix++)
					{
						int cx = x*scale + ix; 
						int cy = y*scale + iy;
						cr = cr + img_0(cx,cy).r ;
                                                cg = cg + img_0(cx,cy).g ;
                                                cb = cb + img_0(cx,cy).b ;
					}
			poly_0(x,y).r = cr/(scale*scale);
			poly_0(x,y).g = cg/(scale*scale);
                        poly_0(x,y).b = cb/(scale*scale);

		}
	
	
	string smallpath = filepath + "/opencvtest/small/";
	string pathpolypfm0 = smallpath + "/" + nef + "small" + ".pfm";
	string pathpolypng0 = smallpath + "/" + nef + "small" + ".png";
	io::exportImage(pathpolypfm0, poly_0);
	io::exportImage(pathpolypng0, poly_0);

	cout << " begin save oriimgdata " << endl;
//	cv::imshow( "window", orimat);
	string pathpolypng1 = smallpath + "/" + nef + "smallopencv" + ".png";
	io::exportImage(pathpolypng1, smallimg);


}



