CXX = g++
CFLAGS = -fimplicit-constexpr -std=c++23 -MMD -MP
INCLUDES := $(shell find . -name '*.cpp')
OBJ := $(patsubst %.cpp,%.o,$(INCLUDES))
TEST := *.md
DEPS := $(OBJ:.o=.d)
DEBUG = -g  -fsanitize=address,leak -Wall -Wextra -pedantic
OUT = ../bin/main
OUTFUZZ = ../bin/mainFuzz
CORES = ./cores/core.*
FUZZOBJ := $(shell find core/ -name '*.cpp')
FUZZFLAGS := -max_len=16365 -only_ascii=1 -print_final_stats=1 -trace_malloc=[12]
FUZZCFLAGS := -g -O1 -fno-omit-frame-pointer -fsanitize-coverage=trace-cmp,trace-gep,trace-div -fno-optimize-sibling-calls -fsanitize=fuzzer,address,leak -DFUZZER  -std=c++23

all: br
cr : clean build run
br : build run

fuzzerClean: clean
	@echo "Compiling"
	@mkdir -p ../bin
	@mkdir -p ../tests/fuzzCorpus
	clang++ $(FUZZCFLAGS) $(FUZZOBJ) fuzz_target.cc -o $(OUTFUZZ)
	@bash ../tests/fuzz.sh
	@$(OUTFUZZ) ../tests/fuzzCorpus/ $(FUZZFLAGS)

fuzzer: clean
	@echo "Compiling"
	@mkdir -p ../bin
	@mkdir -p ../tests/fuzzCorpus
	clang++ $(FUZZCFLAGS) $(FUZZOBJ) fuzz_target.cc -o $(OUTFUZZ)
	# @bash ../tests/fuzz.sh
	@$(OUTFUZZ)  ../tests/fuzzCorpus/ $(FUZZFLAGS)

fuzzerRun:
	@mkdir -p ../bin
	@mkdir -p ../tests/fuzzCorpus
	@$(OUTFUZZ)  ../tests/fuzzCorpus/ $(FUZZFLAGS)

-include $(DEPS)

build: $(OBJ)
	@echo "Linking..."
	@mkdir -p ../bin
	$(CXX) $(CFLAGS) $(OBJ) -o $(OUT) 

%.o: %.cpp 
	@echo "Compiling..."
	$(CXX) -DDEBUG $(CFLAGS) -c $< -o $@  	
	@echo "Compiled:" $@


run: $(OBJ)
	@$(OUT)

debug: 
	@echo "Compiling..."
	@mkdir -p ../bin
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)


dr:
	@echo "Compiling..."
	@mkdir -p ../bin
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)
	@$(OUT)

clean:
	@echo "Deleting..."
	@rm -f $(OBJ) $(OUT) $(TEST) $(CORES)
