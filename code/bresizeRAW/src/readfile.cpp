#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

void readfloattxtfile( string wholeFilesPath, string filename, float data[])
{
    string pathstr = wholeFilesPath + "/" + filename ;
    std::cout << "floattxtfile path: " << pathstr << std::endl;
    char const* path = pathstr.c_str();
    std::ifstream inputfile(path);
    float datatxt = 0;
    int indextxt = 0;
    while(!inputfile.eof())
    {
        inputfile >> datatxt;
        data[indextxt] = datatxt;
        indextxt++;
    }
    inputfile.close();
}

