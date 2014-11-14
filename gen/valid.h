#ifndef VALID_H
#define VALID_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>


#define FOREACH_PROCEDURE(call) { 					\
													\
	(call)(string("wain"));							\
													\
	for(procedures::iterator it = this->p.begin(); 	\
		it != this->p.end(); 						\
		++it) {										\
		if(it->first == "wain") continue;			\
		(call)(it->first);							\
	}												\
}

#define FOREACH_FUNCTION(call) { 					\
													\
	(call)(string("wain"));							\
													\
	for(functions::iterator it = this->p.begin(); 	\
		it != this->p.end(); 						\
		++it) {										\
		if(it->first == "wain") continue;			\
		(call)(it->first);							\
	}												\
}

#define FOREACH(obj) for(int i=0; i<obj.size(); ++i)


using namespace std;

typedef pair<string, vector<string> > node;
typedef map<string, string> cell;
typedef map<string, cell> table;

struct Tree {
	node content;
	vector<Tree> children;
	string type;

	string toString();
	string lhs();
	vector<string> rhs();
	string getType();
	void printTree(int level = 0);
};

bool isTerminal(const string&);
bool isProcedure(const string&);
bool isSymbol(const string&, const string&);

node makeNode(string&);
Tree makeTree(node);
Tree makeTree(node, vector<Tree>);
Tree makeTree(node, Tree);

Tree readTree();

string genSignature(Tree &t);
table* genSymbols(Tree &t);
void testTypes(Tree &t);

void printSymbols();

vector<string>* getOrder();
cell* getSignature();
vector<string>* getOrder();

bool isNum(const string&);
bool isReg(const string&);

#endif