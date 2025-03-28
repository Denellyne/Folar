#include "core/Compiler/compiler.h"
#ifdef FUZZER
#include "fuzz_target.cc"
#endif // FUZZER

int main(int argc, char *argv[]) {
  if (argc > 1) {
    compiler folar(argv[1]);
  } else
    compiler folar("../tests/ok.flr");
  return 0;
}
