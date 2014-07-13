CC = g++

LIB = -O3 -Wall

TARGET = Threes

all:
	${CC} Threes.cpp main.cpp ${LIB} -o ${TARGET}

run1:
	./${TARGET}

clean:
	rm -rf ${TARGET}
