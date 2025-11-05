#include "core/Compiler/compiler.h"
#include "core/ErrorHandler/errorHandler.h"
#include "core/Interpreter/interpreter.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  // for (int i = 0; i < Size; i++) {
  // if (Data[i] < 32 || Data[i] > 127)
  // return -1;
  // }
  std::string str(reinterpret_cast<const char *>(Data), Size);
  str += '\0';
  std::ofstream f("../tests/fuzzCorpus/fuzz.flr");
  f << str;
  f.close();

  bool error = false;
  // std::unique_ptr<compiler> folar =
  // std::make_unique<compiler>("../tests/fuzzCorpus/fuzz.flr");
  interpreter folar("../tests/fuzzCorpus/fuzz.flr");
  error = errorHandler::getInstance().gotErrors();
  errorHandler::getInstance().reset();
  // if (error)
  // return -1;
  return 0; // Values other than 0 and -1 are reserved for future use.
}
