#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>
#include <stdlib.h>  
#include <stdint.h>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cout;
using std::cin;
using std::bitset;
using std::hex;
using std::map;
using std::stringstream;
using std::istringstream;
using std::getline;
using ASM::Token;
using ASM::Lexer;

namespace Bin {
  /**
  * Expression to binary
  */
  int parse(int op, int s, int t, int i) {
    return (op << 26) | (s << 21) | (t << 16) | (i & 0xffff);
  }

  int parse2(int op, int s, int t, int d, int i) {
    return (op << 26) | (s << 21) | (t << 16) | (d << 11) | (i & 0x7ff);
  }

  /**
  * print i in n binary characters
  */
  void output(int i) {
    cout << (char) ((0xff000000 & i) >> 24);
    cout << (char) ((0x00ff0000 & i) >> 16);
    cout << (char) ((0x0000ff00 & i) >> 8);
    cout << (char) (0x000000ff & i);
  }
}

namespace Valid {
  int reg(Token token) {
    if(token.toString() == "REGISTER") {
      string reg = token.getLexeme().substr(1);
      int regInt = atoi(reg.c_str());

      if(regInt > 31) {
        throw string("ERROR: No such register " + token.getLexeme());
      }

      return regInt;
    }
    else {
      throw string("ERROR: expected register, got " + token.toString());
    }
  }
}

int main(int argc, char* argv[]){
  // Nested vector representing lines of Tokens
  // Needs to be used here to cleanup in the case
  // of an exception
  vector< vector<Token*> > tokLines;

  int lineCount = 0;

  map<string, int> labels;

  bool fail = false;

  try{
    // Create a MIPS recognizer to tokenize
    // the input lines
    Lexer lexer;

    // MERL fun times
    tokLines.push_back(lexer.scan("beq $0, $0, 2"));
    tokLines.push_back(lexer.scan(".word endmodule"));
    tokLines.push_back(lexer.scan(".word endcode"));

    // Tokenize each line of the input
    string line;
    while(getline(cin,line)){
      tokLines.push_back(lexer.scan(line));
    }

    stringstream ss;
    vector<vector<Token*> >::iterator it;
    map<int, int> relocation;
    int i = -1;

    // First scan: Labels
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      ++i;

      vector<Token*>::iterator it2;

      for(it2 = it->begin(); it2 != it->end(); ++it2){

        if((*(*it2)).toString() == "DOTWORD") {

          if(++it2 == it->end()) {
            throw string("ERROR: there's no words...");
          }
          else if((*(*it2)).toString() == "INT" || (*(*it2)).toString() == "HEXINT") {
            // Next scan
          }
          else if((*(*it2)).toString() == "ID") {
            if(i > 2)
              relocation[i] = lineCount*4;
          }
          else {
            throw string("ERROR: there's no words...");
          }

          lineCount++;
        }
        else if((*(*it2)).toString() == "ID") {
          if((*(*it2)).getLexeme() == "jr"
            || (*(*it2)).getLexeme() == "jalr"
            || (*(*it2)).getLexeme() == "add"
            || (*(*it2)).getLexeme() == "sub"
            || (*(*it2)).getLexeme() == "mult"
            || (*(*it2)).getLexeme() == "multu"
            || (*(*it2)).getLexeme() == "div"
            || (*(*it2)).getLexeme() == "divu"
            || (*(*it2)).getLexeme() == "mfhi"
            || (*(*it2)).getLexeme() == "mflo"
            || (*(*it2)).getLexeme() == "lis"
            || (*(*it2)).getLexeme() == "lw"
            || (*(*it2)).getLexeme() == "sw"
            || (*(*it2)).getLexeme() == "slt"
            || (*(*it2)).getLexeme() == "sltu"
            || (*(*it2)).getLexeme() == "beq"
            || (*(*it2)).getLexeme() == "bne") {
              lineCount++;
          }
        }
        else if((*(*it2)).toString() == "LABEL"){

          string label = (*(*it2)).getLexeme();
          label.erase(label.end()-1);

          if(labels.count(label) > 0) {
            throw string("ERROR: Redefining label " + label);
          }

          if(it2 != it->begin()) {
            if((*(*(it2-1))).toString() != "LABEL") {
              throw string("ERROR: label " + label + " defined after instruction");
            }
          }

          labels[label] = 4 * lineCount;
        }
      }
    }

    // MEEEEERL
    tokLines.push_back(lexer.scan("endcode:"));
    labels["endcode"] = 4 * lineCount;

    i = 1;
    map<int, int>::iterator iter;
    for(iter = relocation.begin(); iter != relocation.end(); ++iter) {
      string si = SSTR(i);

      tokLines.push_back(lexer.scan(string(".word 1")));
      tokLines.push_back(lexer.scan(string(".word reloc" + si)));

      lineCount += 2;

      labels[string("reloc" + si)] = iter->second;

      i++;
    }

    tokLines.push_back(lexer.scan("endmodule:"));

    labels["endmodule"] = 4 * lineCount;

    lineCount = 0;
    
    // Iterate over the lines of tokens and print them
    // to standard error
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;

      for(it2 = it->begin(); it2 != it->end(); ++it2){

        //cerr << *(*it2) << endl;

        if((*(*it2)).toString() == "DOTWORD") {

          if(++it2 == it->end()) {
            throw string("ERROR: there's no words...");
          }
          else if((*(*it2)).toString() == "ID") {
            string label = (*(*it2)).getLexeme();

            if(labels.count(label) == 0) {
              throw string("ERROR: Undefined label " + label);
            }

            int i = labels[label];

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(i);

          }
          else if((*(*it2)).toString() == "INT" || (*(*it2)).toString() == "HEXINT") {
            int i = (*(*it2)).toInt();

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(i);
          }
          else {
            throw string("ERROR: there's no words...");
          }

          lineCount++;
        }
        else if((*(*it2)).toString() == "LABEL"){
          // Already counted
        }
        else if((*(*it2)).toString() == "ID") {

          if((*(*it2)).getLexeme() == "jr"
            || (*(*it2)).getLexeme() == "jalr"
            || (*(*it2)).getLexeme() == "add"
            || (*(*it2)).getLexeme() == "sub"
            || (*(*it2)).getLexeme() == "mult"
            || (*(*it2)).getLexeme() == "multu"
            || (*(*it2)).getLexeme() == "div"
            || (*(*it2)).getLexeme() == "divu"
            || (*(*it2)).getLexeme() == "mfhi"
            || (*(*it2)).getLexeme() == "mflo"
            || (*(*it2)).getLexeme() == "lis"
            || (*(*it2)).getLexeme() == "lw"
            || (*(*it2)).getLexeme() == "sw"
            || (*(*it2)).getLexeme() == "slt"
            || (*(*it2)).getLexeme() == "sltu"
            || (*(*it2)).getLexeme() == "beq"
            || (*(*it2)).getLexeme() == "bne") {
              lineCount++;
          }


          if((*(*it2)).getLexeme() == "jr") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after jr instruction");
            }

            int s = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, 0, 8));
          }
          else if((*(*it2)).getLexeme() == "jalr") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after jalr instruction");
            }

            int s = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, 0, 9));
          }
          else if((*(*it2)).getLexeme() == "add") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after add instruction");
            }

            int d = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete add instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete add instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            int t = Valid::reg(*(*it2));

            Bin::output(Bin::parse2(0, s, t, d, 32));
          }
          else if((*(*it2)).getLexeme() == "sub") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after sub instruction");
            }

            int d = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete sub instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete sub instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            int t = Valid::reg(*(*it2));

            Bin::output(Bin::parse2(0, s, t, d, 34));
          }
          else if((*(*it2)).getLexeme() == "slt") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after slt instruction");
            }

            int d = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete slt instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete slt instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            int t = Valid::reg(*(*it2));

            Bin::output(Bin::parse2(0, s, t, d, 42));
          }
          else if((*(*it2)).getLexeme() == "sltu") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after sltu instruction");
            }

            int d = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete sltu instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete sltu instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            int t = Valid::reg(*(*it2));

            Bin::output(Bin::parse2(0, s, t, d, 43));
          }

          else if((*(*it2)).getLexeme() == "beq") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after beq instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete beq instruction");
            }

            int t = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete beq instruction");
            }

            if((*(*it2)).toString() == "INT") {
              int i = (*(*it2)).toInt();
              if(i > 32767 || i < -32768) {
                throw string("ERROR: offset out of bounds");
              }
              Bin::output(Bin::parse(4, s, t, i));
            }
            else if((*(*it2)).toString() == "HEXINT") {
              unsigned int j = (*(*it2)).toInt();

              if(j > 65535) {
                throw string("ERROR: constant out of range");
              }

              int16_t i = (*(*it2)).toInt();

              Bin::output(Bin::parse(4, s, t, i));
            }
            else if((*(*it2)).toString() == "ID") {
              string label = (*(*it2)).getLexeme();

              if(labels.count(label) == 0) {
                throw string("ERROR: Undefined label " + label);
              }

              int i = labels[label]/4 - lineCount;
              if(i > 32767 || i < -32768) {
                throw string("ERROR: offset out of bounds");
              }

              Bin::output(Bin::parse(4, s, t, i));
            }
            else {
              throw string("ERROR: incomplete beq instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }
          }
          else if((*(*it2)).getLexeme() == "bne") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after bne instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete bne instruction");
            }

            int t = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA"
                || ++it2 == it->end()) {
              throw string("ERROR: incomplete bne instruction");
            }


            if((*(*it2)).toString() == "INT") {
              int i = (*(*it2)).toInt();
              if(i > 32767 || i < -32768) {
                throw string("ERROR: offset out of bounds");
              }
              Bin::output(Bin::parse(5, s, t, i));
            }
            else if((*(*it2)).toString() == "HEXINT") {
              unsigned int j = (*(*it2)).toInt();

              if(j > 65535) {
                throw string("ERROR: constant out of range");
              }

              int16_t i = (*(*it2)).toInt();

              Bin::output(Bin::parse(5, s, t, i));
            }
            else if((*(*it2)).toString() == "ID") {
              string label = (*(*it2)).getLexeme();

              if(labels.count(label) == 0) {
                throw string("ERROR: Undefined label " + label);
              }

              int i = labels[label]/4 - lineCount;
              if(i > 32767 || i < -32768) {
                throw string("ERROR: offset out of bounds");
              }

              Bin::output(Bin::parse(5, s, t, i));
            }
            else {
              throw string("ERROR: incomplete bne instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }
          }

          else if((*(*it2)).getLexeme() == "lis") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after lis instruction");
            }

            int d = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse2(0, 0, 0, d, 20));
          }

          else if((*(*it2)).getLexeme() == "mflo") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after mflo instruction");
            }

            int d = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse2(0, 0, 0, d, 18));
          }

          else if((*(*it2)).getLexeme() == "mfhi") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after mfhi instruction");
            }

            int d = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse2(0, 0, 0, d, 16));
          }

          else if((*(*it2)).getLexeme() == "lw") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after lw instruction");
            }

            int t = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete lw instruction");
            }

            if(++it2 == it->end() 
                || ((*(*it2)).toString() != "INT" && (*(*it2)).toString() != "HEXINT")) {
              throw string("ERROR: incomplete lw instruction");
            }

            int i = (*(*it2)).toInt();

            if((*(*it2)).toString() == "HEXINT") {
              unsigned int j = (*(*it2)).toInt();
              if(j > 65535) {
                throw string("ERROR: constant out of range");
              }
              int16_t z = (*(*it2)).toInt();
              i = z;
            }
            if(i > 32767 || i < -32768) {
              throw string("ERROR: constant out of range");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "LPAREN") {
              throw string("ERROR: incomplete lw instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete lw instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "RPAREN") {
              throw string("ERROR: incomplete lw instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(35, s, t, i));
          }

          else if((*(*it2)).getLexeme() == "sw") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after sw instruction");
            }

            int t = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete sw instruction");
            }

            if(++it2 == it->end() 
                || ((*(*it2)).toString() != "INT" && (*(*it2)).toString() != "HEXINT")) {
              throw string("ERROR: incomplete sw instruction");
            }

            int i = (*(*it2)).toInt();
            if((*(*it2)).toString() == "HEXINT") {
              unsigned int j = (*(*it2)).toInt();
              if(j > 65535) {
                throw string("ERROR: constant out of range");
              }
              int16_t z = (*(*it2)).toInt();
              i = z;
            }
            if(i > 32767 || i < -32768) {
              throw string("ERROR: constant out of range");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "LPAREN") {
              throw string("ERROR: incomplete sw instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete sw instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "RPAREN") {
              throw string("ERROR: incomplete sw instruction");
            }

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(43, s, t, i));
          }

          else if((*(*it2)).getLexeme() == "mult") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after mult instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete mult instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete mult instruction");
            }

            int t = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, t, 24));
          }

          else if((*(*it2)).getLexeme() == "multu") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after multu instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete multu instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete multu instruction");
            }

            int t = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, t, 25));
          }

          else if((*(*it2)).getLexeme() == "div") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after div instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete div instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete div instruction");
            }

            int t = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, t, 26));
          }

          else if((*(*it2)).getLexeme() == "divu") {
            if(++it2 == it->end()) {
              throw string("ERROR: no register specified after divu instruction");
            }

            int s = Valid::reg(*(*it2));

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "COMMA") {
              throw string("ERROR: incomplete divu instruction");
            }

            if(++it2 == it->end() 
                || (*(*it2)).toString() != "REGISTER") {
              throw string("ERROR: incomplete divu instruction");
            }

            int t = Valid::reg(*(*it2));

            if((it2+1) != it->end()) {
              throw string("ERROR: expected EOL");
            }

            Bin::output(Bin::parse(0, s, t, 27));
          }

          else {
            throw string("ERROR: Unrecognized command " + (*(*it2)).getLexeme());
          }
        }

        else {
          ss.str(string());
          ss << *(*it2);
          throw string("ERROR: Invalid token " + ss.str() + " in this context");
        }
      }
    }

  } catch(const string& msg){
    // If an exception occurs print the message and end the program
    cerr << msg << endl;
    fail = true;
  }
  // Delete the Tokens that have been made
  vector<vector<Token*> >::iterator it;
  for(it = tokLines.begin(); it != tokLines.end(); ++it){
    vector<Token*>::iterator it2;
    for(it2 = it->begin(); it2 != it->end(); ++it2){
      delete *it2;
    }
  }

  // Output symbols table
  if(!fail) {
    map<string, int>::iterator iter;
    for(iter = labels.begin(); iter != labels.end(); ++iter) {
      cerr << iter->first << " " << iter->second << endl;
    }
  }

}
