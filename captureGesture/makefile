all: record.cpp demo.cpp run.cpp
	g++ -o record record.cpp `pkg-config --libs --cflags opencv cvblob`
	g++ -o demo demo.cpp -I/Applications/MATLAB_R2015a.app/extern/include/ -L/Applications/MATLAB_R2015a.app/bin/maci64/ -leng -lm -lmat -lmx -lut 
	g++ -o run run.cpp `pkg-config --libs --cflags opencv cvblob` -I/Applications/MATLAB_R2015a.app/extern/include/ -L/Applications/MATLAB_R2015a.app/bin/maci64/ -leng -lm -lmat -lmx -lut 