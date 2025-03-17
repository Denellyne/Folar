#pragma once
#include <string>
typedef unsigned short tokenId;

#define DEBUG

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
  NOTEQUALToken,
  // Operations
  MULTIPLYToken,
  ADDToken,
  SUBToken,
  DIVIDEToken,
  MODULUSToken,
  ASSIGNToken,
  POINTERToken,
  // Misc
  MUTABLEToken,
  IDENTIFIERToken,
  ENDStatementToken,
  LBracketToken,
  RBracketToken,
  LCurlyBracketToken,
  RCurlyBracketToken,
  TYPEIdentifierToken,

  // Declarations
  IFToken,
  WHILEToken,
  LETToken,
  MULTToken,
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
  STRINGLiteralToken,
  CHARLiteralToken,
};

#ifdef DEBUG

inline const std::string tokenNames[] = {
    "NOToken",
    "ERRORToken",
    "NEWLineToken",
    "EOFToken",
    "BITWISENOTToken",
    "BITWISEANDToken",
    "BITWISEORToken",
    "LSHIFTToken",
    "RSHIFTToken",
    "NOTToken",
    "ANDToken",
    "ORToken",
    "XORToken",
    "LESSToken",
    "GREATToken",
    "LESSEQUALToken",
    "GREATEQUALToken",
    "EQUALToken",
    "NOTEQUALToken",
    "MULTIPLYToken",
    "ADDToken",
    "SUBToken",
    "DIVIDEToken",
    "MODULUSToken",
    "ASSIGNToken",
    "POINTERToken",
    "MUTABLEToken",
    "IDENTIFIERToken",
    "ENDStatementToken",
    "LBracketToken",
    "RBracketToken",
    "LCurlyBracketToken",
    "RCurlyBracketToken",
    "TYPEIdentifierToken",
    "IFToken",
    "WHILEToken",
    "LETToken",
    "MULTToken",
    "RETURNToken",
    "STRUCTToken",
    "INT8Token",
    "INT16Token",
    "INT32Token",
    "INT64Token",
    "UInt8Token",
    "UInt16Token",
    "UInt32Token",
    "UInt64Token",
    "CHARToken",
    "UCharToken",
    "STRINGToken",
    "BOOLToken",
    "FLOAT32Token",
    "FLOAT64Token",
    "FLOAT128Token",
    "VOIDToken",
    "STRINGLiteralToken",
    "CHARLiteralToken",
};

#endif
