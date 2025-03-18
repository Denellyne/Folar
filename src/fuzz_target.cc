#include "core/Lexer/lexer.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  std::string str(*Data, Size);
  std::ofstream f("../tests/fuzzCorpus/fuzz.flr");
  f << str;
  f.close();

  lexer l("../tests/fuzzCorpus/fuzz.flr");
  return 0; // Values other than 0 and -1 are reserved for future use.
}
