CXX=g++
XLOGINXX=xkonec75

OBJ=kry.o
BIN=kry

#CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -lgmp -std=c++20
CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -std=c++17

LINK.o = $(LINK.cpp)

all: CXXFLAGS += -Ofast -march=native -flto
all: kry

debug: CXXFLAGS += -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: kry

kry: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(BIN) -lgmp

generate: kry
	./kry -g 256

encode: kry
	./kry -e 0x26e583ed9677699b25069a079cc133ab7860c946f4c4ae7d46497d33070a9a97 0x6ecd7bff6b240c627a1aa2347e8b7685ece1e2f0dbdae379047bf012d1d7b75d 0x42

decode: kry
	./kry -d 0x49b2bf8439f9c5bd321018003002a16f9608fce795704c4bfa0e04ef69d60b67 0x6ecd7bff6b240c627a1aa2347e8b7685ece1e2f0dbdae379047bf012d1d7b75d 0x426979fa05d348ecae0c47d379fc750f07ae880847dea43d57eaa88d2d9a9194

break: kry
	./kry -b 0x42

pack:
	zip $(XLOGINXX).zip *.cpp *.hpp Makefile doc.pdf

dep:
	g++ *.cpp -MM >> Makefile
