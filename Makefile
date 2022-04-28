CXX=g++
XLOGINXX=xkonec75

OBJ=kry.o
BIN=kry

#CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -lgmp -std=c++2a
CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -std=c++2a

LINK.o = $(LINK.cpp)

all: CXXFLAGS += -Ofast -march=native -flto
all: kry

debug: CXXFLAGS += -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: kry

kry: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(BIN) -lgmp

generate: kry
	./kry -g 16

encode: kry
	./kry -e 0x7237 0x797b 0x42

decode: kry
	./kry -d 0x24a7 0x797b 0x4d2b

break: kry
	./kry -b 0x42

pack:
	zip $(XLOGINXX).zip *.cpp *.hpp Makefile doc.pdf

dep:
	g++ *.cpp -MM >> Makefile
