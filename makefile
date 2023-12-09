CC = cc
LIBS = $(shell pkg-config --libs --cflags raylib)
SRC = main.cpp
OUT = main


# build:
# 	eval cc $(OUT).cpp $(LIBS) -o $(OUT)
# 	./$(OUT)

build:
	clang++ main.cpp -o pong -std=c++11 -I/opt/local/include -L/opt/local/lib -lraylib
	./pong


clean:
	rm -f $(OUT)

