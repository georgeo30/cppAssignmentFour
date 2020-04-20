#include <string>
#include <iostream>
#include <vector>
#include "cluster.h"
#include <sstream>

using namespace std;
namespace THNGEO002
{
// default constructor
Cluster::Cluster(string outputFile,int noOfClusters,int histogramWidth,const char* folderName) : outputFile(outputFile), noOfClusters(noOfClusters),histogramWidth(histogramWidth),dataset(folderName)
{
   
}
 // destructor
Cluster::~Cluster()
{
  

}

//METHOD TO READ THE FILES INTO A VECTOR
void Cluster::readFolder(){
    //METHOD TO READ IN ALL THE PPM FILES THAT ARE INSIDE THE USER INPUT DIRECTORY
    DIR *d;
    struct dirent *dir;
    //OPENING THE DIRECTORY
    d = opendir(dataset);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
           
            fileList.push_back(dir->d_name);
            //FILTERING OUT THE CURRENT AND PARENT DIRECTORY WHICH GETS ADDED FOR SOME REASON
            if(fileList.back()=="."||fileList.back()==".."){
                fileList.pop_back();
            }
        }
        //CLOSING THE DIRECTORY
        closedir(d);
    }
    

}


}