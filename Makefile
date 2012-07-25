exe: rsa Anumber
	g++ -Wall -g -o rsa rsa.o Anumber.o
Anumber: Anumber.cpp
	g++ -Wall -g -c Anumber.cpp
rsa: rsa.cpp
	g++ -Wall -g -c rsa.cpp
clean:
	rm *.o
