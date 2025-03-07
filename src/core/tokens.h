#pragma once
#include <experimental/filesystem>
typedef unsigned char tokenId;

enum tokens {

  NOToken = 0,
  ERRORToken,
  NEWLineToken,
  EOFToken,
  // operators
  BITWISENOTToken,
  BITWISEANDToken,
  BITWISEORToken,
  LSHIFTToken,
  RSHIFTToken,
  // Comparison
  NOTToken,
  ANDToken,
  ORToken,
  XORToken,
  LESSToken,
  GREATToken,
  LESSEQUALToken,
  GREATEQUALToken,
  EQUALToken,
  // Operations
  MULTIPLYToken,
  ADDToken,
  SUBToken,
  DIVIDEToken,
  MODULUSToken,
  ASSIGNToken,
  POINTERToken,
  // Misc
  ENDStatementToken,
  LBracketToken,
  RBracketToken,
  LCurlyBracketToken,
  RCurlyBracketToken,
  TYPEIdentifierToken,

  //Declarations
  IFToken,
  WHILEToken,
  LETToken,
  MULTToken,
  RETURNToken,

  //Types
  INT8Token,
  INT16Token,
  INT32Token,
  INT64Token,
  
  UInt8Token,
  UInt16Token,
  UInt32Token,
  UInt64Token,

  CHARToken,
  UCharToken,

  STRINGToken,
  
  BOOLToken,

  FLOAT32Token,
  FLOAT64Token,
  FLOAT128Token,

  VOIDToken,
};

struct token {

  token() = delete;
  token(tokenId id, unsigned line, unsigned column)
      : id(id), line(line), column(column) {}

  unsigned line = 0;
  unsigned column = 0;
  tokenId id = NOToken;
};
