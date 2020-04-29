#include <string>
#include <iostream>
#include "cluster.h"

using namespace std;

int main(int argc, char *argv[])
{
    string dataset;
    string outputFile;
    int noOfClusters;
    int histogramWidth;
    if (argc > 2)
    {
        dataset = argv[1];
        string s(argv[2]);
        if (s == "-o")
        {
            outputFile = argv[3];
            if (argc > 4)
            {
                string ss(argv[4]);
                if (ss == "-k")
                {
                    noOfClusters = stoi(argv[5]);
                    if (argc > 6)
                    {
                        string sss = argv[6];
                        if (sss == "-bin")
                        {
                            histogramWidth = stoi(argv[7]);
                        }
                        else
                        {
                            cout << "invalid input" << endl;
                        }
                    }
                    else
                    {
                        histogramWidth = 1;
                    }
                }
                else if (ss == "-bin")
                {
                    noOfClusters = 10;
                    histogramWidth = stoi(argv[5]);
                }
                else
                {
                    cout << "invalid input" << endl;
                }
            }
            else
            {
                noOfClusters = 10;
                histogramWidth = 1;
            }
        }
        else if (s == "-k")
        {
            outputFile = "toConsoleDefault";
            noOfClusters = stoi(argv[3]);
            if (argc > 4)
            {
                string ks(argv[4]);
                if (ks == "-bin")
                {
                    histogramWidth = stoi(argv[5]);
                }
                else
                {
                    cout << "invalid input" << endl;
                }
            }
            else
            {
                histogramWidth = 1;
            }
        }
        else if (s == "-bin")
        {
            outputFile = "toConsoleDefault";
            noOfClusters = 10;
            histogramWidth = stoi(argv[3]);
        }
    }
    else
    {
        outputFile = "toConsoleDefault";
        noOfClusters = 10;
        histogramWidth = 1;
    }
    

    THNGEO002::Cluster c(outputFile,noOfClusters,histogramWidth,argv[1]);
    c.readFolder();
    cout<<c;

}