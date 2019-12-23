CPP = g++-9
C = gcc-9
OMP = -fopenmp

all: c.out cpp.out

cpp.out: helloworld.cpp
	${CPP} ${OMP} -o cpp.out helloworld.cpp

c.out: helloworld.c
	${C} ${OMP} -o c.out helloworld.c
