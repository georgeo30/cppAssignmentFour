#include <string>
#include <iostream>
#include <vector>
#include "cluster.h"
#include <sstream>

using namespace std;
namespace THNGEO002
{
// default constructor
Cluster::Cluster(string outputFile, int noOfClusters, int histogramWidth, const char *folderName) : outputFile(outputFile), noOfClusters(noOfClusters), histogramWidth(histogramWidth), dataset(folderName)
{
}
// destructor
Cluster::~Cluster()
{
}

//METHOD TO READ THE FILES INTO A VECTOR
void Cluster::readFolder()
{
    //METHOD TO READ IN ALL THE PPM FILES THAT ARE INSIDE THE USER INPUT DIRECTORY
    DIR *d;
    struct dirent *dir;
    //OPENING THE DIRECTORY
    d = opendir(dataset);
    if (d)
    {
        string fileName;
        while ((dir = readdir(d)) != NULL)
        {

            fileName=dir->d_name;
            //FILTERING OUT THE CURRENT AND PARENT DIRECTORY WHICH GETS ADDED FOR SOME REASON
            if ((fileName != "." )&& (fileName != ".."))
            {
                readEachFile(fileName);
            }
            
        }
        //CLOSING THE DIRECTORY
        closedir(d);
    }
    
}

//METHOD TO READ EACH FILE IN THE VECTOR AND STORE ITS GREYSCALE VALUES
void Cluster::readEachFile(string fileName)
{
    string fName(dataset);
    string sliceUrl ="./"+ fName + "/"+fileName;
    ifstream ppmFile;
    ppmFile.open(sliceUrl, ios::binary);
    while (!ppmFile.eof())
    {
        
         getline(ppmFile,sliceUrl);
         //cout<<sliceUrl<<endl;
    }

    
    ppmFile.close();
    
    
}

} // namespace THNGEO002