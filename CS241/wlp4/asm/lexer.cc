#include "lexer.h"
#include "kind.h"
using std::string;
using std::vector;

// Use the annonymous namespace to prevent external linking
namespace {
  // An array represents the Token kind that each state represents
  // The ERR Kind represents an invalid token
  ASM::Kind stateKinds[] = {
    ASM::ERR,            // ST_ERR
    ASM::ERR,            // ST_START
    ASM::ERR,            // ST_DOLLAR
    ASM::ERR,            // ST_MINUS
    ASM::REGISTER,       // ST_REGISTER
    ASM::INT,            // ST_INT
    ASM::ID,             // ST_ID
    ASM::LABEL,          // ST_LABEL
    ASM::COMMA,          // ST_COMMA
    ASM::LPAREN,         // ST_LPAREN
    ASM::RPAREN,         // ST_RPAREN
    ASM::INT,            // ST_ZERO
    ASM::ERR,            // ST_ZEROX
    ASM::HEXINT,         // ST_HEXINT
    ASM::WHITESPACE,     // ST_COMMENT
    ASM::ERR,            // ST_DOT
    ASM::ERR,            // ST_DOTW
    ASM::ERR,            // ST_DOTWO
    ASM::ERR,            // ST_DOTWOR
    ASM::DOTWORD,        // ST_DOTWORD
    ASM::WHITESPACE      // ST_WHITESPACE
  };
  const string whitespace = "\t\n\r ";
  const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  const string digits = "0123456789";
  const string hexDigits = "0123456789ABCDEFabcdef";
  const string oneToNine =  "123456789";

}

ASM::Lexer::Lexer(){
  // Set default transitions to the Error state
  for(int i=0; i < maxStates; ++i){
    for(int j=0; j < maxTrans; ++j){
      delta[i][j] = ST_ERR;
    }
  }
  // Change transitions as appropriate for the MIPS recognizer
  setTrans( ST_START,      whitespace,     ST_WHITESPACE );
  setTrans( ST_WHITESPACE, whitespace,     ST_WHITESPACE );
  setTrans( ST_START,      letters,        ST_ID         );
  setTrans( ST_ID,         letters+digits, ST_ID         );
  setTrans( ST_START,      oneToNine,      ST_INT        );
  setTrans( ST_INT,        digits,         ST_INT        );
  setTrans( ST_START,      "-",            ST_MINUS      );
  setTrans( ST_MINUS,      digits,         ST_INT        );
  setTrans( ST_START,      ",",            ST_COMMA      );
  setTrans( ST_START,      "(",            ST_LPAREN     );
  setTrans( ST_START,      ")",            ST_RPAREN     );
  setTrans( ST_START,      "$",            ST_DOLLAR     );
  setTrans( ST_DOLLAR,     digits,         ST_REGISTER   );
  setTrans( ST_REGISTER,   digits,         ST_REGISTER   );
  setTrans( ST_START,      "0",            ST_ZERO       );
  setTrans( ST_ZERO,       "x",            ST_ZEROX      );
  setTrans( ST_ZERO,       digits,         ST_INT        );
  setTrans( ST_ZEROX,      hexDigits,      ST_HEXINT     );
  setTrans( ST_HEXINT,     hexDigits,      ST_HEXINT     );
  setTrans( ST_ID,         ":",            ST_LABEL      );
  setTrans( ST_START,      ";",            ST_COMMENT    );
  setTrans( ST_START,      ".",            ST_DOT        );
  setTrans( ST_DOT,        "w",            ST_DOTW       );
  setTrans( ST_DOTW,       "o",            ST_DOTWO      );
  setTrans( ST_DOTWO,      "r",            ST_DOTWOR     );
  setTrans( ST_DOTWOR,     "d",            ST_DOTWORD    );

  // A comment can only ever lead to the comment state
  for(int j=0; j < maxTrans; ++j) delta[ST_COMMENT][j] = ST_COMMENT;
}

// Set the transitions from one state to another state based upon characters in the
// given string
void ASM::Lexer::setTrans(ASM::State from, const string& chars, ASM::State to){
  for(string::const_iterator it = chars.begin(); it != chars.end(); ++it)
    delta[from][static_cast<unsigned int>(*it)] = to;
}

// Scan a line of input (as a string) and return a vector
// of Tokens representing the MIPS instruction in that line
vector<ASM::Token*> ASM::Lexer::scan(const string& line){
  // Return vector
  vector<Token*> ret;
  if(line.size() == 0) return ret;
  // Always begin at the start state
  State currState = ST_START;
  // startIter represents the beginning of the next Token
  // that is to be recognized. Initially, this is the beginning
  // of the line.
  // Use a const_iterator since we cannot change the input line
  string::const_iterator startIter = line.begin();
  // Loop over the the line
  for(string::const_iterator it = line.begin();;){
    // Assume the next state is the error state
    State nextState = ST_ERR;
    // If we aren't done then get the transition from the current
    // state to the next state based upon the current character of
    //input
    if(it != line.end())
      nextState = delta[currState][static_cast<unsigned int>(*it)];
    // If there is no valid transition then we have reach then end of a
    // Token and can add a new Token to the return vector
    if(ST_ERR == nextState){
      // Get the kind corresponding to the current state
      Kind currKind = stateKinds[currState];
      // If we are in an Error state then we have reached an invalid
      // Token - so we throw and error and delete the Tokens parsed
      // thus far
      if(ERR == currKind){
        vector<Token*>::iterator vit;
        for(vit = ret.begin(); vit != ret.end(); ++vit)
          delete *vit;
        throw "ERROR in lexing after reading " + string(line.begin(),it);
      }
      // If we are not in Whitespace then we push back a new token
      // based upon the kind of the State we end in
      // Whitespace is ignored for practical purposes
      if(WHITESPACE != currKind)
        ret.push_back(Token::makeToken(currKind,string(startIter,it)));
      // Start of next Token begins here
      startIter = it;
      // Revert to start state to begin recognizing next token
      currState = ST_START;
      if(it == line.end()) break;
    } else {
      // Otherwise we proceed to the next state and increment the iterator
      currState = nextState;
      ++it;
    }
  }
  return ret;
}
