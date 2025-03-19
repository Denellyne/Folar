#include "core/Lexer/lexer.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  for (int i = 0; i < Size; i++) {
    if (Data[i] < 32 || Data[i] > 127)
      return -1;
  }
  std::string str(reinterpret_cast<const char *>(Data), Size);
  std::ofstream f("../tests/fuzzCorpus/fuzz.flr");
  f << str;
  f.close();

  bool error = false;
  lexer l("../tests/fuzzCorpus/fuzz.flr", error);
  if (error)
    return -1;
  return 0; // Values other than 0 and -1 are reserved for future use.
}
