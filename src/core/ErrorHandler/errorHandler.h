#pragma once
#include <fstream>
#include <string>
#include <vector>
#define ERRORTOKEN 0
#define MALFORMEDSTRING 1

inline const std::string errorTypes[]{"ERRORTOKEN", "MALFORMEDSTRING"};

class errorHandler {
public:
  ~errorHandler();
  void reportError(std::ifstream &file, unsigned line, unsigned column,
                   unsigned filePos, unsigned errorType);

private:
  struct error {
    error() = delete;
    error(std::string str, unsigned line, unsigned column, unsigned errorType)
        : str(str), line(line), column(column), errorType(errorType) {}

    unsigned line, column, errorType;
    const std::string str;

    friend std::ostream &operator<<(std::ostream &os, error const &err) {
      return os << "ERROR TYPE: " << errorTypes[err.errorType]
                << " LINE: " << err.line << " COLUMN: " << err.column << '\n'
                << err.str << '\n';
    }
  };

  const std::string getErrorLine(std::ifstream &file, unsigned filePos,
                                 unsigned column);
  std::vector<error> errors;
};

static errorHandler errorReport;
