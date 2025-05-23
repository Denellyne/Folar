#pragma once
#include "tokenEnum.h"
#include <unordered_map>

static std::unordered_map<std::string, tokenId> keywords{
    {"if", IFToken},
    {"mut", MUTABLEToken},
    {"while", WHILEToken},
    {"struct", STRUCTToken},
    {"let", LETToken},
    {"return", RETURNToken},
    {"int8", INT8Token},
    {"int16", INT16Token},
    {"int32", INT32Token},
    {"int64", INT64Token},
    {"uint8", UInt8Token},
    {"uint16", UInt16Token},
    {"uint32", UInt32Token},
    {"uint64", UInt64Token},
    {"char", CHARToken},
    {"uchar", UCharToken},
    {"float32", FLOAT32Token},
    {"float64", FLOAT64Token},
    {"float128", FLOAT128Token},
    {"string", STRINGToken},
    {"bool", BOOLToken},
    {"true", TRUEToken},
    {"false", FALSEToken},
    {"void", VOIDToken},
    {"enum", ENUMToken},
    {"null", NULLToken}};
