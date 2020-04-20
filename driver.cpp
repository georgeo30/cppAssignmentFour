#include <string>
#include <iostream>
#include "cluster.h"
using namespace std;

int main(int argc, char *argv[])
{
    string outputFile;
    int noOfClusters;
    int histogramWidth;
    if (argc>2)
    {
        string s(argv[2]);
        if (s=="-o")
        {
            outputFile=argv[3];
            
        }
        else if(s=="-k"){
            noOfClusters=stoi(argv[3]);
        }
        else if( s=="-bin"){
            histogramWidth=stoi(argv[3]);
        }
        
    }
    cout<<outputFile;
    cout<<noOfClusters;
    
}