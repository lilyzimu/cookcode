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
    
    int imgStart = 7; // + 1  = 1. 
    int imgEnd = 10;
    
    string nefname = "scene";
    string nefformat = ".CR2";   
    // make all JPGs according to gtLightingFactors
    #pragma omp parallel for 
    for ( int i = imgStart; i < imgEnd; i++ )
    {
	stringstream testi;
	testi <<(i+1);
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
	
	
	string smallpath = filepath + "test/";
	string pathpolypfm0 = smallpath + "/" + nef + "small" + ".pfm";
	string pathpolypng0 = smallpath + "/" + nef + "small" + ".png";
	io::exportImage(pathpolypfm0, poly_0);
	io::exportImage(pathpolypng0, poly_0);
/*
	string pathimg0png = filepath+ "/" + nef + ".png";
	io::exportImage(pathimg0png, img_0);
	string pathimg0pfm = filepath + "/" + nef + ".pfm";
	io::exportImage(pathimg0pfm, img_0);	
// 	// print out the pixel values to txt files
// 	std::ofstream rawpixeltxt, polypixeltxt;
// 	string rawpixeltxtpathstr = filename + "/" + nef + ".txt";
// 	char const* rawpixeltxtpath = rawpixeltxtpathstr.c_str();
// 	rawpixeltxt.open(rawpixeltxtpath);
// 	string polypixeltxtpathstr = filename + "/" + nef + "Curve" + selectCurveNumstr + "th" + ".txt";
// 	char const* polypixeltxtpath = polypixeltxtpathstr.c_str();
// 	polypixeltxt.open(polypixeltxtpath);
// 	for( int x = 0; x < img_0.width(); x++ )
// 		for( int y = 0; y < img_0.height(); y++ )
// 		{	
// 			rawpixeltxt << lightingfactor*img_0(x,y).r << " " << lightingfactor*img_0(x,y).g << " "<< lightingfactor*img_0(x,y).b << " ";
// 			polypixeltxt << poly_0(x,y).r << " " << poly_0(x,y).g << " " << poly_0(x,y).b << " ";
// 		}
// 	rawpixeltxt.close();
// 	polypixeltxt.close();
*/

}



