#include "errorHandler.h"
#include <iostream>

errorHandler::~errorHandler() {
  if (errors.empty())
    return;

  for (const auto &err : errors)
    std::cerr << err;

  std::cerr << "Found " << errors.size() << " errors\n";
  reset();
}
bool errorHandler::gotErrors() { return errors.size() != 0; }
void errorHandler::reset() { errors.clear(); }

void errorHandler::reportError(std::ifstream &file, unsigned line,
                               unsigned column, unsigned filePos,
                               unsigned errorType) {
  const std::string str = getErrorLine(file, filePos, column);
  errors.emplace_back(error(std::move(str), line, column, errorType));
}

void errorHandler::reportError(std::ifstream &file, unsigned line,
                               unsigned column, unsigned filePos,
                               unsigned errorType, std::string_view str) {

  std::string strLine = getErrorLine(file, filePos, column);
  errors.emplace_back(error(std::move(strLine), line, column, errorType, str));
}
void errorHandler::reportError(std::string &str, unsigned line, unsigned column,
                               unsigned errorType) {
  errors.emplace_back(str, line, column, errorType);
}
void errorHandler::reportError(unsigned errorType) {
  errors.emplace_back(error(errorType));
}
void errorHandler::reportError(unsigned errorType, std::string_view str) {
  errors.emplace_back(error(errorType, str));
}
void errorHandler::reportError(unsigned errorType, const std::string str) {
  errors.emplace_back(error(errorType, str));
}

const std::string errorHandler::getErrorLine(std::ifstream &file,
                                             unsigned filePos,
                                             unsigned &column) {

  std::string str = "";
  file.seekg(filePos - column, std::ios_base::beg);
  std::getline(file, str);
  while (str[0] == ' ') {
    column--;
    str = str.substr(1);
  }

  file.seekg(filePos);
  return str;
}
