#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype> 

using namespace std;

bool isNum(const string& input) {

	if(input != "0") {
		for(int i=0; i<input.length(); ++i) {
			if(!isdigit(input[i])) {
				return 0;
			}
		}
		if(input.length() > 1 && input[0] == '0')
			return 0;
	}

	return 1;
}

bool isId(const string& input) {

	if(isalpha(input[0])) {
		for(int i=0; i<input.length(); ++i) {
			if(!isalnum(input[i])) {
				return 0;
			}
		}
		return 1;
	}

	return 0;
}

string kind(const string& input) {
	if(input == "NULL")
		return "NULL";
	else if(input == "&")
		return "AMP";
	else if(input == "]")
		return "RBRACK";
	else if(input == "[")
		return "LBRACK";
	else if(input == "delete")
		return "DELETE";
	else if(input == "new")
		return "NEW";
	else if(input == ";")
		return "SEMI";
	else if(input == ",")
		return "COMMA";
	else if(input == "%")
		return "PCT";
	else if(input == "/")
		return "SLASH";
	else if(input == "*")
		return "STAR";
	else if(input == "-")
		return "MINUS";
	else if(input == "+")
		return "PLUS";
	else if(input == ">=")
		return "GE";
	else if(input == "<=")
		return "LE";
	else if(input == ">")
		return "GT";
	else if(input == "<")
		return "LT";
	else if(input == "!=")
		return "NE";
	else if(input == "==")
		return "EQ";
	else if(input == "int")
		return "INT";
	else if(input == "=")
		return "BECOMES";
	else if(input == "wain")
		return "WAIN";
	else if(input == "println")
		return "PRINTLN";
	else if(input == "while")
		return "WHILE";
	else if(input == "else")
		return "ELSE";
	else if(input == "if")
		return "IF";
	else if(input == "return")
		return "RETURN";
	else if(input == "}")
		return "RBRACE";
	else if(input == "{")
		return "LBRACE";
	else if(input == ")")
		return "RPAREN";
	else if(input == "(")
		return "LPAREN";
	else if(input == "" || input == "\r" || input == " "|| input == "\n" || input == "\t")
		return "WHITESPACE";
	else if(isId(input))
		return "ID";
	else if(isNum(input))
		return "NUM";
	else
		return "ERROR";
}

string stripComments(const string& input) {
	int find = input.find("//");
	
	if(find == -1)
		return input;

	return input.substr(0, find) + " ";
}

bool isSet1(const string& input) {
	if(input == "ID" || input == "NUM" || input == "RETURN" || input == "IF" || input == "ELSE" || input == "WHILE" || input == "PRINTLN" || input == "WAIN" || input == "INT" || input == "NEW" || input == "NULL" || input == "DELETE")
		return 1;
	return 0;
}

bool isSet2(const string& input) {
	if(input == "EQ" || input == "NE" || input == "LT" || input == "LE" || input == "GT" || input == "GE" || input == "BECOMES")
		return 1;
	return 0;
}

int main() {
	string line, current, currentKind, previous;
	vector<pair<string, string> > ret, final;

	try {
		while(getline(cin, line)) {
			current = currentKind = "";
			line = stripComments(line);
			for(int i=0; i<line.length(); ++i) {
				currentKind = kind(current);
				previous = current;

				current.push_back(line[i]);
				if(kind(current) == "ERROR") {
					if(currentKind == "ERROR")
						throw string("double error");

					ret.push_back(make_pair(currentKind, previous));
					current = string(1, line[i]);
				
				}
			}
			ret.push_back(make_pair(kind(current), current));
			ret.push_back(make_pair("WHITESPACE", ""));
		}
		ret.push_back(make_pair("WHITESPACE", ""));

		for(int i=0; i<ret.size()-1; ++i) {
			if((isSet1(ret[i].first) && isSet1(ret[i+1].first)) || (isSet2(ret[i].first) && isSet2(ret[i+1].first))){
				throw string("fail");
			}
			if(ret[i].first != "WHITESPACE") {
				final.push_back(ret[i]);
			}
		}
		for(int i=0; i<final.size(); ++i) {
			cout << final[i].first << " " << final[i].second << endl;
		}
	}
	catch(string err) {
		cerr << "ERROR : " + err << endl;
	}

}