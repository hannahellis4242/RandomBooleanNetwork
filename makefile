all : main

clean :
	rm main *.o

run : main
	./main

main : main.o Network.o
	g++ --std=c++2a $^ -o $@

%.o : %.cpp
	g++ --std=c++2a -c $<
