#pragma once

#include <sstream>
#include <vector>
using namespace std;

enum class TokenType {
  DATE,
  EVENT,
  COLUMN,
  LOGICAL_OP,
  COMPARE_OP,
  PAREN_LEFT,
  PAREN_RIGHT,
};

struct Token {
  const string value;
	//DATE looks like "20170101"
	//EVENT looks like "sport event" (without parentheses but with spaces)
	//COLUMN means either 'date' or 'event'
	//LOGICAL_OP means either 'AND' or 'OR'
	//COMPARE_OP means '>' or '>=' or '==' or '!=' or '<=' or '<'

	const TokenType type;
};

vector<Token> Tokenize(istream& cl);
