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
    int c = 0;
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
                //cout<<fileName<<endl;
                fileNameVector.push_back(fileName);
                readEachFile(fileName, c);
                c++;
            }
        }
        //CLOSING THE DIRECTORY
        closedir(d);
    }

    imageFeature();
    //  for (int i = 0; i < 32; i++)
    // {
    //     for (int r = 0; r < 32; r++)
    //     {
    //          cout<<fileList[0][i][r]*1<<" ";
    //     }
    //     cout<<endl;
    // }
    initializeClusters();
    adjustMeans();
}

//METHOD TO READ EACH FILE IN THE VECTOR AND STORE ITS GREYSCALE VALUES
void Cluster::readEachFile(string fileName, int c)
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
            fileList.push_back(new int *[Nrows]);
            for (int j = 0; j < Nrows; j++)
            {
                fileList[c][j] = new int[Ncols];
                for (int k = 0; k < Ncols; k++)
                {
                    int gs = (0.21 * ppmFile.get()) + (0.72 * ppmFile.get()) + (0.07 * ppmFile.get());
                    //cout<<gs;

                    fileList[c][j][k] = gs;
                }
                //cout<<endl;
            }
            ppmFile.get();
        }
    }

    ppmFile.close();
}
//METHOD TO CREATE THE HISTOGRAM IMAGE FEATURE
void Cluster::imageFeature()
{

    histogramSize = ceil(256 / histogramWidth);
    for (int imageC = 0; imageC < fileList.size(); imageC++)
    {
        histogramArray.push_back(new int[histogramSize]);
        for (int k = 0; k < histogramSize; k++)
        {
            histogramArray[imageC][k] = 0;
        }

        for (int i = 0; i < Nrows; i++)
        {
            for (int j = 0; j < Ncols; j++)
            {
                int val = floor(fileList[imageC][i][j] / histogramWidth);

                histogramArray[imageC][val]++;
            }
        }
    }

    // for (int i = 0; i < histogramArray.size(); i++)
    //     {
    //         cout << "[ ";
    //         for (int j = 0; j < histogramSize; j++)
    //         {
    //             cout << histogramArray[i][j] << " ";
    //         }
    //         cout << " ]\n";

    //     }
}
void Cluster::initializeClusters()
{
    //POPULATING THE CLUSTER MATRIX WITH INITIAL HISTOGRAMS
    for (int i = 0; i < noOfClusters; i++)
    {
        int ranVal = rand() % (histogramArray.size());
        vector<int> temp;
        meansZZ.push_back(ranVal);
        temp.push_back(ranVal);
        matrix.resize(noOfClusters);
    }
    //LOOPING THROUGH THE HISTOGRAMS TO ADD TO CLUSTERS
    for (int i = 0; i < histogramArray.size(); i++)
    {
        //CLUSTER NUMBER THAT THE IMAGE WILL BELONG TOO
        int clusterItBelongsToo;
        //INITIAL MEAN
        double mean = (__DBL_MAX__);
        //LOOPING THROUGH THE NUMBER OF PIXELS
        for (int j = 0; j < matrix.size(); j++)
        {
            int bSquareTotal = 0;
            //LOOPING THROUGH THE PIXELS AND PERFORMING CALCULATION
            for (int k = 0; k < histogramSize; k++)
            {

                int val = histogramArray[meansZZ[j]][k] - histogramArray[i][k];
                int square = val * val;
                bSquareTotal += square;
            }

            double distance = sqrt(bSquareTotal);

            //CONDITION TO SEE IF IT IS SMALLER THAN PREVIOUS CLUSTER
            if (distance <= mean)
            {
                //IF SMALLER THEN SET NEW MEAN TO THIS AND JOIN THIS CLUSTER
                mean = distance;
                clusterItBelongsToo = j;
            }
        }
        //ASSIGN TO THE CORRESPONDING CLUSTER
        matrix[clusterItBelongsToo].push_back(i);
    }

    // for (int i = 0; i < matrix.size(); i++)
    // {
    //     for (int j = 0; j < matrix[i].size(); j++)
    //     {
    //         cout << i << " " << j << " =====> " << matrix[i][j] << endl;
    //     }
    //     cout << endl
    //          << endl;
    // }
}

void Cluster::adjustMeans()
{
    for (int i = 0; i < noOfClusters; i++)
    {
        clusterMeans.push_back(new double[histogramSize]);
        for (int k = 0; k < histogramSize; k++)

        {
            double total = 0;

            for (int j = 0; j < matrix[i].size(); j++)
            {
                total += histogramArray[matrix[i][j]][k];
            }
            clusterMeans[i][k] = total / matrix[i].size();
        }
    }
}

} // namespace THNGEO002