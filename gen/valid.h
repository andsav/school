#ifndef VALID_H
#define VALID_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>

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
table genSymbols(Tree &t);
void testTypes(Tree &t);

void printSymbols();
vector<string> getOrder();
cell getSignature();

#endif