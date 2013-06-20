objs = JsonParser.o filter.o 

headers = JsonParser.h 

filter : $(objs) $(headers)
	g++ -o filter JsonParser.o filter.o

filter.o : filter.cpp
	g++ -c --std=c++0x filter.cpp

JsonParser.o : JsonParser.cpp JsonParser.h
	g++ -c --std=c++0x JsonParser.cpp 

clean :
	rm -f filter *.o

