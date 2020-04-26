#ifndef _CLUSTER_H
#define _CLUSTER_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <stdio.h>
#include <cstdlib>
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
    int histogramSize;
    std::vector<string> fileNameVector;
    
    std::vector<int **> fileList;
    std::vector<int *>histogramArray;
    std::vector<std::vector<int>> matrix;
    std::vector<double> meansZZ;
        std::vector<int *>clusterMeans;

    
public:                                   // public members
    Cluster(string outputFile,int noOfClusters,int histogramWidth,const char* dataset);                           //constructor - define in .cpp
    ~Cluster();                          // destructor - define in .cpp file
    void readFolder(); //method to read files from the folder into a vector
    void readEachFile(std::string fileName,int c); //method to read each file and store ts greyscale equivalent    
    void imageFeature(); //method to create the histogram image feature
    void initializeClusters(); //initialize clusters INITIALIZATION STEP 
    void adjustMeans();
};

} // namespace THNGEO002
#endif