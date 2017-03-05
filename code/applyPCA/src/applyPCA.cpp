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

//add pieter lab
#include "image.h"
#include "imageIO.h"
#include "color.h"

#include "displaytime.h"
#include "readfile.h"


using namespace std;

void applyPCAsub( string filename, string nef, string nefformat, vector<vector<vector<vector<double> > > >& basisPCA_batch, vector<vector<vector<double> > >& meanPCA_batch,  float selectCurvePoly[], int pcacoefNum , int selectCurveNum, int exposure, int WIDTH, int HEIGHT);


void applyPCA()
{
	/// make the RAW to JPGs & pfms
    	std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
    	string filepath = "/big/temp/hanli/data/moredata/zztestscene/";
   
 
    	// read pca data
	int pcaCoefsNum = 25;
	int pcaRange = 1024;
	int curveNum = 201;
    	float meanlist[pcaRange], eigenlist[pcaCoefsNum*pcaRange], curvelist[curveNum*pcaCoefsNum];
	string txtpath = filepath + "txt";
 	string meanlistpath = "meanOfBrightnessBYmatlabPCA.txt";
	string eigenlistpath = "realeigenvector1024BYmatlabPCA25FromRAWtoJPG.txt";
	string curvelistpath = "pcacoefsFor201curvesFromRAWtoJPG.txt";
	readfloattxtfile(txtpath, meanlistpath, meanlist);
        readfloattxtfile(txtpath, eigenlistpath, eigenlist);
        readfloattxtfile(txtpath, curvelistpath, curvelist);

	float meanpcavector[pcaRange], eigenvector[pcaCoefsNum][pcaRange], curvepcacoefs[curveNum][pcaCoefsNum];
	for( int i = 0; i <pcaRange; i++ )
		meanpcavector[i] = meanlist[i];

	for(int i = 0; i < pcaCoefsNum; i++)
		for(int j = 0; j<pcaRange; j++)
			eigenvector[i][j] = eigenlist[i*pcaRange+j];
	for(int i = 0; i < curveNum; i++)
		for(int j = 0; j< pcaCoefsNum; j++)
			curvepcacoefs[i][j] = curvelist[i*pcaCoefsNum+j];


///////////////////////////////////////////////////////////////////////

    int imgStart = 0; // + 1  = 257. 
    int imgEnd = 10;
    int expolist[3] = { 10, 7, 5};
	
    string pcacurvepath = filepath + "pcacurve/";
    string smallpath = filepath + "small/";    
    string nefname = "scene";
    string nefformat = "small.pfm";   
    // make all JPGs according to gtLightingFactors
    #pragma omp parallel for 
    for ( int i = imgStart; i < imgEnd; i++ )
    {
     std::cout << "begin image " << i << ": " << std::endl;
     for( int expoindex = 0; expoindex < 3; expoindex++)
     {
	int exposure = expolist[expoindex];
	stringstream testi;
	testi <<(i+1);
	string currentnefname = nefname + testi.str();
	image img_0, img_1;
        string filepathimg0 = smallpath + currentnefname + nefformat;
        io::importImage(filepathimg0, img_0);

	int WIDTH = img_0.width();
	int HEIGHT = img_0.height();
	int DEPTH = 3;

	vector<vector<vector<vector<double> > > > basisPCA; 
	vector<vector<vector<double> > > meanPCA;
	meanPCA.resize(WIDTH);
	basisPCA.resize(WIDTH);
	for(int i = 0; i< WIDTH; i++)
	{
		meanPCA[i].resize(HEIGHT);
		basisPCA[i].resize(HEIGHT);
		for(int j =0; j< HEIGHT; j++)
		{
			meanPCA[i][j].resize(DEPTH);
			basisPCA[i][j].resize(DEPTH);
			for(int k = 0; k< DEPTH; k++)
				basisPCA[i][j][k].resize(pcaCoefsNum);	
		}
	}

	for(int y = 0; y < img_0.height(); y++ )
                for(int x = 0; x < img_0.width(); x++ )
                {
			int r = img_0(x,y).r * pcaRange * exposure / 10.0;
                        int g = img_0(x,y).g * pcaRange * exposure / 10.0;
                        int b = img_0(x,y).b * pcaRange * exposure / 10.0;
			if(r>=pcaRange) r = pcaRange-1;
                        if(g>=pcaRange) g = pcaRange-1;
                        if(b>=pcaRange) b = pcaRange-1;
			meanPCA[x][y][0] = meanpcavector[r];
                        meanPCA[x][y][1] = meanpcavector[g];
                        meanPCA[x][y][2] = meanpcavector[b];
			for(int k = 0; k< pcaCoefsNum; k++)
			{
				basisPCA[x][y][0][k] = eigenvector[k][r];
                                basisPCA[x][y][1][k] = eigenvector[k][g];
                                basisPCA[x][y][2][k] = eigenvector[k][b];

			}
		}
	
	for(int curveindex = 0; curveindex < curveNum; curveindex++)
	{
		float selectCurvePoly[pcaCoefsNum];
		for ( int i = 0; i < pcaCoefsNum; i++ )
			selectCurvePoly[i] = curvepcacoefs[curveindex][i];
		applyPCAsub(pcacurvepath, currentnefname, nefformat, basisPCA, meanPCA, selectCurvePoly, pcaCoefsNum, curveindex, exposure, WIDTH, HEIGHT);
	}
     }

    }
    std::cout << "makeJpgs success: " << "currentDateTime()=" << currentDateTime() << std::endl;
    //////////////////////////////////////////////////////////


}

void applyPCAsub( string filename, string nef, string nefformat, vector<vector<vector<vector<double> > > >& basisPCA_batch, vector<vector<vector<double> > >& meanPCA_batch,  float selectCurvePoly[], int pcacoefNum , int selectCurveNum, int exposure, int WIDTH, int HEIGHT)
{
//      std::cout<< "apply pca sub: " << std::endl;
	image poly_0(WIDTH, HEIGHT);

        for(int y = 0; y < HEIGHT; y++ )
                for(int x = 0; x < WIDTH; x++ )
                {
                        float r = 0.0, g = 0.0, b = 0.0;

                        for(int i = 0; i< pcacoefNum; i++)
                        {
                                r = r + basisPCA_batch[x][y][0][i] * selectCurvePoly[i];
                                g = g + basisPCA_batch[x][y][1][i] * selectCurvePoly[i];
                                b = b + basisPCA_batch[x][y][2][i] * selectCurvePoly[i];
                        }
                        r = r + meanPCA_batch[x][y][0];
                        g = g + meanPCA_batch[x][y][1];
                        b = b + meanPCA_batch[x][y][2];
                        if(r>1.0) r = 1.0;
                        if(r<0.0) r = 0.0;
                        if(g>1.0) g = 1.0;
                        if(g<0.0) g = 0.0;
                        if(b>1.0) b = 1.0;
                        if(b<0.0) b = 0.0;
                        poly_0(x,y).r = r;
                        poly_0(x,y).g = g;
                        poly_0(x,y).b = b;
                }


        stringstream curvestr;
        curvestr << (selectCurveNum+1);
        string selectCurveNumstr = curvestr.str();
//        stringstream imgstr;
//      imgstr << imgSeqNum;
//      string imgSeqNumstr = imgstr.str();
        stringstream expostrconvert;
        expostrconvert << exposure;
        string expostr = expostrconvert.str();

//        string pathpolypfm0 = filename + nef + "smallcurve" + selectCurveNumstr  + "exposure" + expostr +  ".pfm";
        string pathpolypng0 = filename + nef + "smallcurve" + selectCurveNumstr  + "exposure" + expostr +  ".png";
//        io::exportImage(pathpolypfm0, poly_0);
        io::exportImage(pathpolypng0, poly_0);
}
