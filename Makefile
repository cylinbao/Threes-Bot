CC = g++

LIB = -O3 -Wall -g3

SOURCE = Threes.cpp main.cpp

TARGET = Threes

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

run:
	./${TARGET}

clean:
	rm -rf ${TARGET}
