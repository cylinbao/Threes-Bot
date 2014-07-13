CC = g++

LIB = -O3 -Wall

all:
	${CC} Threes.cpp main.cpp ${LIB} -o Threes

run1:
	./${TARGET}

clean:
	rm -rf ${TARGET}
