all: run.cpp
	@export MATLAB=/Applications/MATLAB_R2014a.app
	@export PATH=$PATH:/Applications/MATLAB_R2014a.app/bin
	@export PATH=$PATH:/Applications/MATLAB_R2014a.app/bin/maci64
	@export DYLD_LIBRARY_PATH=/Applications/MATLAB_R2015a.app/bin/maci64
	@g++ -o run run.cpp `pkg-config --libs --cflags opencv cvblob` -I/Applications/MATLAB_R2015a.app/extern/include/ -L/Applications/MATLAB_R2015a.app/bin/maci64/ -leng -lm -lmat -lmx -lut 