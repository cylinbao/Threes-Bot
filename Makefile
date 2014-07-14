CC = g++

LIB = -O3 -Wall

<<<<<<< HEAD
SOURCE = Threes.cpp main.cpp

TARGET = Threes

all:
	${CC} ${SOURCE} ${LIB} -o ${TARGET}
=======
TARGET = Threes

all:
	${CC} Threes.cpp main.cpp ${LIB} -o ${TARGET}
>>>>>>> c710d1cb1be25842d9eef704e012483608a56a55

run:
	./${TARGET}

clean:
	rm -rf ${TARGET}
