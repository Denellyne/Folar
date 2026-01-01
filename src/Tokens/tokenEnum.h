#pragma once
#include <array>
typedef unsigned short tokenId;
#define stringify(x) #x

enum tokens {
  NOToken = 0,
  ERRORToken,
  NEWLineToken,
  EOFToken,
  // operators
  BITWISEXORToken,
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
  NOTEQUALToken,
  // Operations
  MULTIPLYToken,
  ADDToken,
  SUBToken,
  DIVIDEToken,
  MODULUSToken,
  POWToken,
  ASSIGNToken,
  POINTERToken,
  // Misc
  // MAINToken,
  // FUNCTIONToken,
  MUTABLEToken,
  IDENTIFIERToken,
  ENDStatementToken,
  ENUMToken,
  LBracketToken,
  RBracketToken,
  LCurlyBracketToken,
  RCurlyBracketToken,
  LRectBracketToken,
  RRectBracketToken,
  TYPEIdentifierToken,

  // Declarations
  IFToken,
  WHILEToken,
  LETToken,
  RETURNToken,

  // Types
  STRUCTToken,
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
  TRUEToken,
  FALSEToken,
  NULLToken,
  STRINGLiteralToken,
  CHARLiteralToken,
  NUMBERLiteralToken,
  FLOATLiteralToken,
};

constexpr std::array<tokens, 16> tokenTypes = {
    INT8Token,    INT16Token,   INT32Token,    INT64Token,
    UInt8Token,   UInt16Token,  UInt32Token,   UInt64Token,
    CHARToken,    UCharToken,   STRINGToken,   BOOLToken,
    FLOAT32Token, FLOAT64Token, FLOAT128Token, VOIDToken,
};
