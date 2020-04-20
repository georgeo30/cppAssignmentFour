CC=g++
LIB=-std=c++11
create: cluster.h Cluster.cpp driver.cpp
	$(CC) $(LIB) cluster.h Cluster.cpp driver.cpp -c
	$(CC) $(LIB) driver.o Cluster.o -o clusterer

run:
	./clusterer MRI

clean:
	rm Cluster.o
	rm driver.o
	rm cluster.h.gch
	rm clusterer
