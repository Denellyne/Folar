#pragma once
typedef unsigned char tokenId;

enum tokens {

  NOToken = 0,
  ERRORToken,
  EOFToken
};

struct token {

  token() = delete;
  token(tokenId id, unsigned line, unsigned column)
      : id(id), line(line), column(column) {}

  unsigned line = 0;
  unsigned column = 0;
  tokenId id = NOToken;
};
