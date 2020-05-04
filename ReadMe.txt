Read Me
---------------------------------------------------

Makefile Details:

make : to compile all cpp and header files, please type make into the command line. it will create driver.o Cluster.o and clusterer executable.


make clean : this will remove all .o files and the executable 

There is no run method for the make as the user needs to type in the foldername

---------------------------------------------------

To Run the program through terminal.

1)Assuming you have already typed in "make" and compiled the files

2) you can then test by typing in as specified in the pdf

    Default :: ./clusterer FolderWithPPMS <===this will run the default with 10 clusters and a bin size of 1 and write output to the screen

    Example :: ./clusterer FolderWithPPMS -o outputfilename -k 6 -bin 4  <====this will run the program with 6 clusterers and a bin size of 4 and write the results to outputfilenamee.txt

    Example :: ./clusterer FolderWithPPMS -k 6 -bin 2 <=== will run the program with 6 clusters and bin size of 2 and write to the console

    ** PLEASE NOT THAT IT YOU WANT TO SPECIFY THE NO OF CLUSTERS AND BIN SIZE. YOU CANNOT SPECIFY THE BIN SIZE BEFORE THE NUMBER OF CLUSTERS ****
    ** THIS MEANS YOU CANT SAY "./clusterer FolderWithPPMS -bin 2 -k 6" INSTEAD YOU HAVE TO SAY "./clusterer FolderWithPPMS -k 6 -bin 2"
    ** THIS IS THE SAME CASE WITH ANY PARAMETERS YOU WANT TO SPECIFY **

    -o >> -k >> -bin

    ** This means you dont havee to specify all of them( if you want the default for some) but if you are specifying more than one then it has to be in this order.


---------------------------------------------------

File structure

driver.cpp : contains the main method which is used to take in the command line input and process accordingly.
Cluster.cpp : This class file contains methods to perform LLOYD'S ALGORITHM
cluster.h : header file for the class Cluster.cpp


---------------------------------------------------

Git Details:

attached within the tar.gz is the .git repo
If this does not open or causes errors to run. Please view the repo on my gitHub which i will make available to public few days after submissions is closed.
GitHub Link : https://github.com/georgeo30/cppAssignmentFour