#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
enum errorTypeEnum {
  ERRORTOKEN = 0,
  MALFORMEDSTRING,
  FILEERROR,
  MALFORMEDTOKEN,
  MALFORMEDCHAR,
  BADESCAPING,
  MALFORMEDNUMBER,
  MALFORMEDEXPR,
};

inline const std::string errorTypes[]{"Unknown token",
                                      "Malformed String literal",
                                      "Failed to open file",
                                      "Malformed Token did you mean ",
                                      "Malformed Character literal",
                                      "Bad escaped sequence",
                                      "Malformed number token",
                                      "Malformed expression"};

class errorHandler {
public:
  ~errorHandler();
  void reset();
  bool gotErrors();
  void reportError(unsigned errorType);
  void reportError(unsigned errorType, const std::string_view str);
  void reportError(unsigned errorType, const std::string str);
  void reportError(std::ifstream &file, unsigned line, unsigned column,
                   unsigned filePos, unsigned errorType);
  void reportError(std::string &str, unsigned line, unsigned column,
                   unsigned errorType);

private:
  struct error {
    error() = delete;
    error(unsigned errorType) : errorType(errorType) {}
    error(unsigned errorType, const std::string_view str)
        : errorType(errorType), str(str) {}
    error(unsigned errorType, const std::string str)
        : errorType(errorType), str(str) {}
    error(std::string str, unsigned line, unsigned column, unsigned errorType)
        : str(std::move(str)), line(line), column(column),
          errorType(errorType) {}

    unsigned line, column, errorType;
    const std::string str;

    friend std::ostream &operator<<(std::ostream &os, error const &err) {
      auto lengthOfInt = [&]() {
        unsigned size = 0;
        unsigned x = err.line;
        while (x > 0) {
          size++;
          x /= 10;
        }
        return size;
      };
      switch (err.errorType) {
      case FILEERROR:
        return os << errorTypes[FILEERROR] << ' ' << err.str << "\n\n";
      default:

        return os << "Error found at line: " << err.line
                  << " column: " << err.column << '\n'
                  << err.line << "| " << err.str << '\n'
                  << std::setw(lengthOfInt() + err.column + 2) << '^'
                  << std::setw(0)
                  << " Error Type: " << errorTypes[err.errorType] << "\n\n";
      }
    }
  };

  const std::string getErrorLine(std::ifstream &file, unsigned filePos,
                                 unsigned &column);
  std::vector<error> errors;
};
static errorHandler errorReport;
