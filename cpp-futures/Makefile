all: prodcon promise 

prodcon: prodcon.cpp
	c++ -std=c++17 prodcon.cpp -o prodcon -lpthread

promise: prodcon-promise.cpp
	c++ -std=c++17 prodcon-promise.cpp -o prodcon -lpthread -g

clean:
	rm -f prodcon prodcon *.o
