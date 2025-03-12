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
  errors.emplace_back(
      error(getErrorLine(file, filePos, column), line, column, errorType));
}

const std::string errorHandler::getErrorLine(std::ifstream &file,
                                             unsigned filePos,
                                             unsigned column) {
  std::string str = "";
  file.seekg(filePos - column);
  std::getline(file, str);
  file.seekg(filePos);
  return str;
}
