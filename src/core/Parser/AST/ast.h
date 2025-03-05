#pragma once
#include "../../tokens.h"

struct node {
  node() = delete;
  node(tokenId id) : id(id) {}
  node(tokenId id, node *left, node *right)
      : id(id), left(left), right(right) {}

  void setLeft(node *node) { left = node; }
  void setRight(node *node) { right = node; }
  node *getLeft() { return left; }
  node *getRight() { return right; }

  node *left, *right;
  tokenId id;
};
