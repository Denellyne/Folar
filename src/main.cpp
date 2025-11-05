#include "core/Compiler/compiler.h"
#include "core/ErrorHandler/errorHandler.h"
#include "core/Interpreter/interpreter.h"
#ifdef FUZZER
#include "fuzz_target.cc"
#endif // FUZZER

int main(int argc, char *argv[]) {
  // if (argc > 1)
  //   interpreter folar(argv[1]);
  // else
  //   compiler folar("../tests/ok.flr");
  if (argc > 1) {
    compiler folar(argv[1]);
    if (errorHandler::getInstance().gotErrors())
      return 1;
    return 0;
  }

  std::cerr << "No file to parse\n";
  return 1;
}
