CXX = g++
CFLAGS = -fimplicit-constexpr -std=c++20
INCLUDES := $(shell find . -name '*.cpp')
OBJ := $(patsubst %.cpp,%.o,$(INCLUDES))
TEST := *.md
DEBUG = -g -Wall -Wextra -pedantic
OUT = ../bin/main
CORES = ./cores/core.*

all: run
cr : clean run

build: $(OBJ)
	@echo "Linking..."
	$(CXX) $(CFLAGS) $(OBJ) -o $(OUT) 

%.o: %.cpp 
	@echo "Compiling..."
	$(CXX) -DDEBUG $(CFLAGS) -c $< -o $@  	
	@echo "Compiled:" $@

run: $(OBJ)
	@echo "Linking..."
	$(CXX) $(CFLAGS) $(OBJ) -o $(OUT) 
	@$(OUT)

debug: 
	@echo "Compiling..."
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)

fuzzer: clean
	@echo "Compiling"
	clang -O1 -fsanitize=fuzzer,address -DFUZZER -g -std=c++20 ./core/*/*.cpp fuzz_target.cc -o $(OUT)
	@mkdir -p ../tests/fuzzCorpus
	@$(OUT) ../tests/fuzzCorpus/ -max_len=16365 -only-ascii=1 -print_final_stats=1

dr:
	@echo "Compiling..."
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)
	@$(OUT)

clean:
	@echo "Deleting..."
	@rm -f $(OBJ) $(OUT) $(TEST) $(CORES)
