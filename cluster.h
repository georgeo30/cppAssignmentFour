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
#include <ctime>    // For time()
#include <chrono>
#include <random>
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
    
    std::vector<int **> fileList;
    std::vector<int *>histogramArray;
    
    std::vector<double> meansZZ;
        std::vector<double *>clusterMeans;
                std::vector<double *>clusterMeansCompare;


    
public:                                   // public members
std::vector<std::vector<int>> matrix;
    std::vector<string> fileNameVector;

    Cluster(string outputFile,int noOfClusters,int histogramWidth,const char* dataset);                           //constructor - define in .cpp
    ~Cluster();                          // destructor - define in .cpp file
    void readFolder(); //method to read files from the folder into a vector
    void readEachFile(std::string fileName,int c); //method to read each file and store ts greyscale equivalent    
    void imageFeature(); //method to create the histogram image feature
    void initializeClusters(); //initialize clusters INITIALIZATION STEP 
    void adjustMeans();//adjusting the means of tthe clusters
    void iterationCluster();//process to get new image distance and place in new cluster
    void copyMeans();//copying the means tocompare.
    bool compareMeans();
    friend ostream& operator<<(ostream& os, const Cluster& c);

};
ostream& operator<<(ostream& os, const Cluster& c);

} // namespace THNGEO002
#endif