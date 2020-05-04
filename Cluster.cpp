#include <string>
#include <iostream>
#include <vector>
#include "cluster.h"
#include <sstream>
#include <cmath>
#include <algorithm>

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
    for (int i = 0; i < fileList.size(); i++)
    {
        for (int j = 0; j < Nrows; j++)
        {
            delete fileList[i][j];
        }
        delete fileList[i];
    }
    for (int i = 0; i < histogramArray.size(); i++)
    {
        delete histogramArray[i];
    }
    for (int i = 0; i < clusterMeans.size(); i++)
    {
        delete clusterMeansCompare[i];
        delete clusterMeans[i];
    }
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

    initializeClusters();
    adjustMeans();
    bool check = true;
    while (check == true)
    {

        copyMeans();

        iterationCluster();
        adjustMeans();
        check = compareMeans();
    }
}

//METHOD TO READ EACH FILE IN THE VECTOR AND STORE ITS GREYSCALE VALUES
void Cluster::readEachFile(const string fileName, int c)
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
            track++;
        }
        //CHECKS FOR SECOND LINE BUT TAKES THE COMMENT LINES INTO ACCOUNT
        else if (track == 1)
        {
            getline(ppmFile, line);

            //CHEECKING TO SEE WHETHER IT IS A COMMENT LINE
            if (line.at(0) == '#')
            {
            }
            //IF NOT COMMENT LINE THEN EXTRACT THE ROWS AND COLUMNS
            else
            {

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
                    //CONVERSION TO GREYSCALE
                    int gs = (0.21 * ppmFile.get()) + (0.72 * ppmFile.get()) + (0.07 * ppmFile.get());
                    //STORING GREYSCALE VALUE
                    fileList[c][j][k] = gs;
                }
            }
            ppmFile.get();
        }
    }

    ppmFile.close();
}
//METHOD TO CREATE THE HISTOGRAM IMAGE FEATURE
void Cluster::imageFeature()
{
    //GETTING TO KNOW THE HISTOGRAM SIZE
    histogramSize = ceil(256 / histogramWidth);
    //LOOPINNG THROUGH THE IMAGES
    for (int imageC = 0; imageC < fileList.size(); imageC++)
    {
        histogramArray.push_back(new int[histogramSize]);
        //ASSIGNING ZERO TO ALL THE ELEMENTS IN HISTOGRAM BECAUSE C++ IS WEIRD
        for (int k = 0; k < histogramSize; k++)
        {
            histogramArray[imageC][k] = 0;
        }
        //INNER LOOP TO LOOP THROUGH THE PIXEL(GREYSCALE VALUE) AND PERFORM INTEGER DIVISION TO KNOW WHICH
        //ELEMENT TO INCREMENT
        for (int i = 0; i < Nrows; i++)
        {
            for (int j = 0; j < Ncols; j++)
            {
                int val = floor(fileList[imageC][i][j] / histogramWidth);

                histogramArray[imageC][val]++;
            }
        }
    }
}
void Cluster::initializeClusters()
{
    //POPULATING THE CLUSTER MATRIX WITH INITIAL HISTOGRAMS
    srand(time(0));
    std::vector<int> numbers;
    //INITIALIZING A NUMBERS VECTOR WITH ITS OWN INDEXES AS ITS VALUES
    for (int i = 0; i < fileNameVector.size(); i++)
    {
        numbers.push_back(i);
    }
    // SHUFFELING THE NUMBERS VECTOR SO THAT A TRUE RANDOM IMAGE CAN BE PICKED
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
    //CHOOSING A RANDOM HISTOGRAM  FOR EACH CLUSTER
    for (int i = 0; i < noOfClusters; i++)
    {

        meansZZ.push_back(numbers[i]);
        //EACH CLUSTER HAS AN HISTOGRAM
        clusterMeans.push_back(new double[histogramSize]);
    }
    //RESIZING HE MATRIX TO THE NUMBER OF CLUSTERS
    matrix.resize(noOfClusters);

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
}
//METHOD TO ADJUST THE MEANS
void Cluster::adjustMeans()
{
    //LOOPING THROUGH THE NUMBER OF CLUSTERS
    for (int i = 0; i < noOfClusters; i++)
    {
        //LOOPING THROUGH HISTOGRAM SIZE TO GET ACCESS TO EACH INDEX
        for (int j = 0; j < histogramSize; j++)

        {
            double total = 0;
            if (matrix[i].size() == 0)
            {

                clusterMeans[i][j] = 0;
            }
            else
            {
                //LOOPING THROUGH THE IMAGES IN THE CLUSTERS TO GET THE MEAN
                for (int k = 0; k < matrix[i].size(); k++)
                {
                    total += histogramArray[matrix[i][k]][j];
                }
                clusterMeans[i][j] = total / matrix[i].size();
            }
        }
    }
}

//ITERATION LOOP TO GET THE NEW CLUSTERS
void Cluster::iterationCluster()
{
    matrix.clear();
    matrix.resize(noOfClusters);
    //LOOPING THROUGH THE IMAGES
    for (int i = 0; i < histogramArray.size(); i++)
    {
        int clusterItBelongsToo;
        double mean = (__DBL_MAX__);
        //LOOPING THROUGH THE NO OF CLUSTERS
        for (int j = 0; j < matrix.size(); j++)
        {

            double meanVal = 0;
            //LOOPING THROUGH THE HISTOGRAM
            for (int k = 0; k < histogramSize; k++)
            {

                int val = histogramArray[i][k] - clusterMeans[j][k];
                int square = val * val;
                meanVal += square;
            }
            double distance = sqrt(meanVal);
            //CONDITION TO SEE IF IT IS SMALLER THAN PREVIOUS CLUSTER
            if (distance <= mean)
            {
                //IF SMALLER THEN SET NEW MEAN TO THIS AND JOIN THIS CLUSTER
                mean = distance;
                clusterItBelongsToo = j;
            }
        }

        matrix[clusterItBelongsToo].push_back(i);
    }
}
//COPYING THE MEANS INTO A DUPLICATE VECTOR FOR COMPARISION IN FUTURE
void Cluster::copyMeans()
{
    for (int i = 0; i < clusterMeans.size(); i++)
    {
        clusterMeansCompare.push_back(new double[histogramSize]);
        for (int j = 0; j < histogramSize; j++)
        {
            clusterMeansCompare[i][j] = clusterMeans[i][j];
        }
    }
}
//COMPARING THE MEANS OF THE PREVIOUS AND CURRENT
bool Cluster::compareMeans()
{
    for (int i = 0; i < clusterMeans.size(); i++)
    {
        for (int j = 0; j < histogramSize; j++)
        {
            if (!(clusterMeansCompare[i][j] == clusterMeans[i][j]))
            {

                return true;
            }
        }
    }
    return false;
}

} // namespace THNGEO002

//OPERATOR OVERLOADING
ostream &THNGEO002::operator<<(ostream &os, const THNGEO002::Cluster &c)
{
    for (int i = 0; i < c.matrix.size(); i++)
    {

        os << "Cluster: " << i << " : ";
        for (int j = 0; j < c.matrix[i].size(); j++)
        {
            if (j == c.matrix[i].size() - 1)
            {
                os << c.fileNameVector[c.matrix[i][j]];
            }
            else
            {
                os << c.fileNameVector[c.matrix[i][j]] << ", ";
            }
        }
        os << endl;
    }
    return os;
}
