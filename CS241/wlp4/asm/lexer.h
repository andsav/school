#ifndef __LEXER_H__
#define __LEXER_H__
#include <string>
#include <vector>

namespace ASM{
  // The different states the the MIPS recognizer uses
  // Judicious use of the pimpl idiom or the Bridge pattern
  // would allow us to place this in the implementation file
  // However, that's more complexity than is necessary
  enum State {
    ST_ERR,
    ST_START,
    ST_DOLLAR,
    ST_MINUS,
    ST_REGISTER,
    ST_INT,
    ST_ID,
    ST_LABEL,
    ST_COMMA,
    ST_LPAREN,
    ST_RPAREN,
    ST_ZERO,
    ST_ZEROX,
    ST_HEXINT,
    ST_COMMENT,
    ST_DOT,
    ST_DOTW,
    ST_DOTWO,
    ST_DOTWOR,
    ST_DOTWORD,
    ST_WHITESPACE
  };
  // Forward declare the Token class to reduce compilation dependencies
  class Token;

  // Class representing a MIPS recognizer
  class Lexer {
    // At most 21 states and 256 transitions (max number of characters in ASCII)
    static const int maxStates = 21;
    static const int maxTrans = 256;
    // Transition function
    State delta[maxStates][maxTrans];
    // Private method to set the transitions based upon characters in the
    // given string
    void setTrans(State from, const std::string& chars, State to);
  public:
    Lexer();
    // Output a vector of Tokens representing the Tokens present in the
    // given line
    std::vector<Token*> scan(const std::string& line);
  };
}

#endif