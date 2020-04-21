#include <string>
#include <iostream>
#include <vector>
#include "cluster.h"
#include <sstream>
#include <cmath>

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
    int c=0;
    d = opendir(dataset);
    if (d)
    {
        string fileName;
        while ((dir = readdir(d)) != NULL)
        {

            fileName = dir->d_name;
            //FILTERING OUT THE CURRENT AND PARENT DIRECTORY WHICH GETS ADDED FOR SOME REASON
            if ((fileName != ".") && (fileName != ".."))
            {
                cout<<fileName<<endl;
                readEachFile(fileName,c);
                c++;
            }
        }
        //CLOSING THE DIRECTORY
        closedir(d);
    }
    
      for (int i = 0; i < 32; i++)
    {
        for (int r = 0; r < 32; r++)
        {
             cout<<fileList[0][i][r]*1<<" ";
        }
        cout<<endl;
    }
    imageFeature();
}

//METHOD TO READ EACH FILE IN THE VECTOR AND STORE ITS GREYSCALE VALUES
void Cluster::readEachFile(string fileName,int c)
{
    string fName(dataset);
    string sliceUrl = "./" + fName + "/" + fileName;
    ifstream ppmFile;
    string line;
    ppmFile.open(sliceUrl, ios::binary);
    int track = 0;
    while (!ppmFile.eof())
    {
        //CHECKS FOR FIRST LINE
        if (track == 0)
        {
            getline(ppmFile, line);
            //cout << "First line is" << line << endl;
            track++;
        }
        //CHECKS FOR SECOND LINE BUT TAKES THE COMMENT LINES INTO ACCOUNT
        else if (track == 1)
        {
            getline(ppmFile, line);

            //CHEECKING TO SEE WHETHER IT IS A COMMENT LINE
            if (line.at(0) == '#')
            {
                //cout << "comment line in header" << endl;
            }
            //IF NOT COMMENT LINE THEN EXTRACT THE ROWS AND COLUMNS
            else
            {

                //cout << "second line is" << line << endl;
                //GETTING THE NROWS AND NCOLUMNS FROM THE HEADER INFORMATION
                std::istringstream ss(line);
                std::string token;
                ss >> Nrows;
                ss >> Ncols;
                track++;
            }
        }
        //EXTRACTING THE INTENSITY RANGE
        else if (track == 2)
        {
            getline(ppmFile, line);
            //cout << "Third line is" << line << endl;
            track++;
        }
        //NOW WE CAN GATHER THE BLOCK BYTES
        else
        {
            
            fileList.push_back(new  unsigned char *[Nrows]);
            for (int j = 0; j < Nrows; j++)
            {
                fileList[c][j] = new unsigned char[Ncols];
                for (int k = 0; k < Ncols; k++)
                {
                    int gs= (0.21 * (ppmFile.get())) + (0.72 * (ppmFile.get()) + (0.07 * (ppmFile.get())));
                    //cout<<gs;
                    fileList[c][j][k]=gs;
                }
                //cout<<endl;
            }
            ppmFile.get();

        }
       
    }

    ppmFile.close();
     
}
//METHOD TO CREATE THE HISTOGRAM IMAGE FEATURE
void Cluster::imageFeature(){
    int range =round(256.0/histogramWidth);
    int histogram[range]={0};
    
        for (int j = 0; j < Nrows; j++)
        {
            for (int  k= 0; k < Ncols; k++)
            {
                histogram[fileList[0][j][k]/histogramWidth]++;
            }
            
        }
        
    
    
    for (int i = 0; i < range; i++)
    {
        cout<<i<<" -----------> "<<histogram[i]<<endl;
    }
    
}
} // namespace THNGEO002