#include "errorHandler.h"
#include <iostream>

errorHandler::~errorHandler() {
  if (errors.empty())
    return;

  for (const auto &err : errors)
    std::cerr << err;

  std::cerr << "Found " << errors.size() << " errors\n";
}

void errorHandler::reportError(std::ifstream &file, unsigned line,
                               unsigned column, unsigned filePos,
                               unsigned errorType) {
  const std::string str = getErrorLine(file, filePos, column);
  errors.emplace_back(error(std::move(str), line, column, errorType));
}
void errorHandler::reportError(unsigned errorType) {
  errors.emplace_back(error(errorType));
}
void errorHandler::reportError(unsigned errorType, std::string_view str) {
  errors.emplace_back(error(errorType, str));
}

const std::string errorHandler::getErrorLine(std::ifstream &file,
                                             unsigned filePos,
                                             unsigned &column) {
  std::string str = "";
  file.seekg(filePos - column);
  while (file.peek() == ' ') {
    column--;
    file.get();
  }
  std::getline(file, str);
  file.seekg(filePos);
  return str;
}
