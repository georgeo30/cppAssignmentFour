#ifndef _CLUSTER_H
#define _CLUSTER_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <stdio.h>
namespace THNGEO002
{
    using namespace std;

class Cluster
{
private:                                  // private members
    const char* dataset;
    string outputFile;
    int noOfClusters;
    int histogramWidth;
    int Nrows;
    int Ncols;
    
    std::vector<int **> fileList;
    std::vector<int *>histogramArray;
    
public:                                   // public members
    Cluster(string outputFile,int noOfClusters,int histogramWidth,const char* dataset);                           //constructor - define in .cpp
    ~Cluster();                          // destructor - define in .cpp file
    void readFolder(); //method to read files from the folder into a vector
    void readEachFile(std::string fileName,int c); //method to read each file and store ts greyscale equivalent    
    void imageFeature(); //method to create the histogram image feature
};

} // namespace THNGEO002
#endif