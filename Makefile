CC = g++

LIB = -O3 -Wall

SOURCE = Threes.cpp main.cpp

TARGET = Threes

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}

run:
	./${TARGET}

clean:
	rm -rf ${TARGET}
