#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
//#include <cassert>

using namespace std;

typedef pair<string, vector<string> > node;
typedef map<string, string> cell;
typedef map<string, cell> table;

string current = "";
table symbols;
cell signature;
vector<string> order;

bool isTerminal(const string&);
bool isProcedure(const string&);
bool isSymbol(const string&, const string&);

struct tree {
	node content;
	vector<tree> children;
	string type;

	string toString() {
		string ret = this->content.first;
		for(int i=0; i<this->content.second.size(); ++i) {
			ret += string(" ") + this->content.second[i];
		}
		return ret;
	}

	string lhs() {
		return this->content.first;
	}

	vector<string> rhs() {
		return this->content.second;
	}

	string getType() {
		//cerr << this->toString() << endl;

		if(!this->type.empty())
			return this->type;

		string ret = "";

		try {
			if(this->lhs() == "ID") {
				if(isSymbol(current, this->rhs()[0])) {
					ret = symbols[current][this->rhs()[0]];
				}
				else if(isProcedure(this->rhs()[0])) {
					ret = signature[this->rhs()[0]];
				}
				else {
					ret = string("You shouldn't ever see this"); 
				}
			}
			if(this->lhs() == "NUM") {
				ret = string("int");
			}
			else if(this->lhs() == "NULL") {
				ret = string("int*");
			}
			else if(this->lhs() == "factor") {
				if(this->rhs().size() == 1 && (this->rhs()[0] == "ID" || this->rhs()[0] == "NUM" || this->rhs()[0] == "NULL")) {
					ret = this->children[0].getType();
				}
				else if(this->toString() == "factor LPAREN expr RPAREN") {
					ret = this->children[1].getType();
				}
				else if(this->toString() == "factor AMP lvalue") {
					if(this->children[1].getType() != "int") {
						throw string("Expected int, got " + this->children[1].getType());
					}
					ret = "int*";
				}
				else if(this->toString() == "factor STAR factor") {
					if(this->children[1].getType() != "int*") {
						throw string("Expected int*, got " + this->children[1].getType());
					}
					ret = "int";
				}
				else if(this->toString() == "factor NEW INT LBRACK expr RBRACK") {
					if(this->children[3].getType() != "int") {
						throw string("Expected int, got " + this->children[1].getType());
					}
					ret = "int*";
				}
				else if(this->toString() == "factor ID LPAREN RPAREN") {
					if(!signature[this->children[0].rhs()[0]].empty()) {
						throw string("Expected procedure " + this->children[0].rhs()[0] + " to have empty signature");
					}
					ret = "int";
				}
				else if(this->toString() == "factor ID LPAREN arglist RPAREN") {
					if(signature[this->children[0].rhs()[0]] != this->children[2].getType()) {
						throw string("Expected procedure " + this->children[0].rhs()[0] + " called with invalid argumets");
					}
					ret = "int";
				}
			}
			else if(this->lhs() == "arglist") {
				if(this->toString() == "arglist expr") {
					ret = this->children[0].getType();
				}
				else {
					ret = string(this->children[0].getType() + " " + this->children[2].getType());
				}
			}
			else if(this->lhs() == "lvalue") {
				if(this->rhs()[0] == "ID") {
					ret = this->children[0].getType();
				}
				else if(this->toString() == "lvalue LPAREN lvalue RPAREN") {
					ret = this->children[1].getType();
				}
				else if(this->toString() == "lvalue STAR factor") {
					if(this->children[1].getType() != "int*") {
						throw string("Expected int*, got " + this->children[1].getType());
					}
					ret = "int";
				}
			}
			else if(this->lhs() == "term") {
				if(this->toString() == "term factor") {
					ret = this->children[0].getType();
				}
				else {
					for(int i=0; i<this->rhs().size(); ++i) {
						if(this->rhs()[i] == "factor" || this->rhs()[i] == "term") {
							if(this->children[i].getType() != "int") {
								throw string("Expected int, got int*");
							}
						}
					}
					ret = "int";
				}
			}
			else if(this->lhs() == "expr") {
				if(this->toString() == "expr term") {
					ret = this->children[0].getType();
				}
				else if(this->toString() == "expr expr PLUS term") {
					if(this->children[0].getType() == "int" && this->children[2].getType() == "int") {
						ret = "int";
					}
					else if(this->children[0].getType() == "int*" && this->children[2].getType() == "int") {
						ret = "int*";
					}
					else if(this->children[0].getType() == "int" && this->children[2].getType() == "int*") {
						ret = "int*";
					}
					else {
						throw string("Illegal addition");
					}
				}
				else if(this->toString() == "expr expr MINUS term") {
					if(this->children[0].getType() == "int" && this->children[2].getType() == "int") {
						ret = "int";
					}
					else if(this->children[0].getType() == "int*" && this->children[2].getType() == "int") {
						ret = "int*";
					}
					else if(this->children[0].getType() == "int*" && this->children[2].getType() == "int*") {
						ret = "int";
					}
					else {
						throw string("Illegal substraction");
					}
				}
				else if(this->toString() == "expr expr STAR term"
					|| this->toString() == "expr expr SLASH term"
					|| this->toString() == "expr expr PCT term") {
					if(this->children[0].getType() == "int" && this->children[2].getType() == "int") {
						ret = "int";
					}
					else {
						throw string("Illegal operation");
					}
				}
			}
			else if(this->lhs() == "main") {
				int i = 0;
				while(this->rhs()[++i] != "dcl");
				while(this->rhs()[++i] != "dcl");

				if(this->children[i].children[0].toString() != "type INT") {
					throw string("Expected type int for the second argument of wain");
				}

				i = 0;
				while(this->rhs()[++i] != "expr");

				if(this->children[i].getType() != "int") {
					throw string("Main is expected to return an int");
				}
			}
			else if(this->lhs() == "procedure") {
				int i = 0;
				while(this->rhs()[++i] != "expr");

				if(this->children[i].getType() != "int") {
					throw string("Procedures are expected to return an int");
				}
			}
			else if(this->lhs() == "statement") {
				if(this->toString() == "statement lvalue BECOMES expr SEMI") {
					if(this->children[0].getType() != this->children[2].getType()) {
						throw string("Assignment type mismatch");
					}
				}
				else if(this->toString() == "statement PRINTLN LPAREN expr RPAREN SEMI") {
					if(this->children[2].getType() != "int") {
						throw string("Expected int for println");
					}
				}
				else if(this->toString() == "statement DELETE LBRACK RBRACK expr SEMI") {
					if(this->children[3].getType() != "int*") {
						throw string("Expected int* for delete[]");
					}
				}
			}
			else if(this->toString() == "dcls dcls dcl BECOMES NULL SEMI") {
				if(this->children[1].children[0].toString() != "type INT STAR") {
					throw string("Type mismatch");
				}
			}
			else if(this->toString() == "dcls dcls dcl BECOMES NUM SEMI") {
				if(this->children[1].children[0].toString() != "type INT") {
					throw string("Type mismatch");
				}
			}
			else if(this->lhs() == "test") {
				if(this->children[0].getType() != this->children[2].getType()) {
					throw string("Comparison between expressions whose type doesn't match");
				}
			}
		}
		catch(string err) {
			throw string("[type error in " + current + "] " + err);
		}

		this->type = ret;
		return ret;
	}

	// Testing fun
	void printTree(int level = 0) {
		for(int i=0; i<level; ++i) {
			cout << " ";
		}
		cout << this->toString() << endl;
		for(int i=0; i<this->children.size(); ++i) {
			this->children[i].printTree(level+1);
		}
	}
};

node makeNode(string&);
tree makeTree(node);
tree makeTree(node, vector<tree>);
tree makeTree(node, tree);

tree readTree();

string genSignature(tree &t);
void genSymbols(tree &t);
void testTypes(tree &t);

void printSymbols();


int main() {
	tree parseTree = readTree();
	try{
		genSymbols(parseTree);
		testTypes(parseTree);
		//printSymbols();
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
}

tree readTree() {
	tree ret;
	string s;

	getline(cin, s);
	ret = makeTree(makeNode(s));

	if(!isTerminal(ret.lhs())) {
		for(int i=0; i<ret.rhs().size(); ++i) {
			ret.children.push_back(readTree());
		}
	}

	return ret;
}

node makeNode(string &s) {
	stringstream ss(s);
	string word;
	node ret;

	ss >> ret.first;
	while(ss >> word) {
		ret.second.push_back(word);
	}
	return ret;
}

tree makeTree(node n, vector<tree> t) {
	tree ret = { n, t };
	return ret;
}

tree makeTree(node n, tree t) {
	vector<tree> vn;
	vn.push_back(t);
	
	return makeTree(n, vn);
}

tree makeTree(node n) {
	vector<tree> vn;
	return makeTree(n, vn);
}

bool isTerminal(const string &s) {
	return (s == "BOF" 
		|| s == "BECOMES" 
		|| s == "COMMA"
		|| s == "ELSE"
		|| s ==  "EOF"
		|| s == "EQ"
		|| s == "GE"
		|| s == "GT"
		|| s == "ID"
  		|| s == "IF"
  		|| s == "INT"
  		|| s == "LBRACE"
  		|| s == "LE"
  		|| s == "LPAREN"
  		|| s == "LT"
  		|| s == "MINUS"
  		|| s == "NE"
  		|| s == "NUM"
  		|| s == "PCT"
  		|| s == "PLUS"
  		|| s == "PRINTLN"
  		|| s == "RBRACE"
  		|| s == "RETURN"
  		|| s == "RPAREN"
  		|| s == "SEMI"
  		|| s == "SLASH"
  		|| s == "STAR"
  		|| s == "WAIN"
  		|| s == "WHILE"
  		|| s == "AMP"
  		|| s == "LBRACK"
  		|| s == "RBRACK"
  		|| s == "NEW"
  		|| s == "DELETE"
  		|| s == "NULL");
}

bool isProcedure(const string& p) {
	return (bool)symbols.count(p);
}

bool isSymbol(const string& p, const string& id) {
	return (bool)symbols[p].count(id);
}

string genSignature(tree &t) {
	string ret = "";

	if(t.toString() == "dcl type ID") {
		if(t.children[0].toString() == "type INT")
			ret = " int";
		else if(t.children[0].toString() == "type INT STAR")
			ret = " int*";
	}

	for(int i=0; i<t.children.size(); ++i) {
		ret += genSignature(t.children[i]);
	}
	return ret;
}

void genSymbols(tree &t) {
	string id, type, sign;
	int lbound, rbound;

	if(t.lhs() == "procedures") {
		if(t.rhs()[0] == "main") {
			current = "wain";
	
			lbound = 0;
			while(t.children[0].rhs()[++lbound] != "LPAREN");

			rbound = lbound;
			while(t.children[0].rhs()[++rbound] != "RPAREN");

			sign = "";
			for(int i=lbound; i<rbound; ++i) {
				sign += genSignature(t.children[0].children[i]);
			}

			signature[current] = sign.erase(0,1);
		}
		else {
			current = t.children[0].children[1].rhs()[0];
			if(isProcedure(current)) {
				throw string("procedure '" + current + "' declared twice");
			}
			signature[current] = genSignature(t.children[0].children[3]).erase(0,1);
		}

		symbols.insert(make_pair(current, cell()));
		order.push_back(current);
	}
	else if(t.toString() == "dcl type ID") {
		id = t.children[1].rhs()[0];

		if(isSymbol(current, id))
			throw string("symbol '" + id + "' declared twice in procedure '" + current + "'");

		if(t.children[0].toString() == "type INT")
			type = "int";
		else if(t.children[0].toString() == "type INT STAR")
			type = "int*";

		symbols[current][id] = type;
	}
	else if(t.toString() == "factor ID LPAREN arglist RPAREN" && !isProcedure(t.children[0].rhs()[0])) {
		throw string("procedure '" + t.children[0].rhs()[0] + "' called but not declared in procedure '" + current + "'");
	}
	else if(t.toString() == "factor ID" && !isSymbol(current, t.children[0].rhs()[0])) {
		throw string("variable '" + t.children[0].rhs()[0] + "' used but not declared in procedure '" + current + "'");
	}
	else if(t.toString() == "factor ID LPAREN arglist RPAREN" && isSymbol(current, t.children[0].rhs()[0])) {
		throw string("ambiguous use of '" + t.children[0].rhs()[0] + "' in procedure '" + current + "'");
	}
	
	for(int i=0; i<t.children.size(); ++i) {
		genSymbols(t.children[i]);
	}
}

void testTypes(tree &t) {
	if(t.lhs() == "procedures") {
		if(t.rhs()[0] == "main") {
			current = "wain";
		}
		else {
			current = t.children[0].children[1].rhs()[0];
		}
	}

	t.getType();

	for(int i=0; i<t.children.size(); ++i) {
		testTypes(t.children[i]);
	}
}

void printSymbols() {
	for(int i=0; i<order.size(); ++i) {
		cerr << order[i] << " " << signature[order[i]] << endl;

		for(cell::const_iterator it2 = symbols[order[i]].begin(); it2 != symbols[order[i]].end(); ++it2) {
			cerr << it2->first << " " << it2->second << endl;
		}

		if(order[i] != "wain")
			cerr << endl;
	}
}
