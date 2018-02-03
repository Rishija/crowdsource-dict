OBJ = main.o dict.o 

all : $(OBJ) dict.h
	g++ -std=gnu++11 -o dict ${OBJ}

%.o : %.cpp dict.h
	g++ -std=gnu++11 -c -o $@ $<

clean :
	rm *.o myDict dict