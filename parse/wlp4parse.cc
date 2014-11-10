#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;

struct prodRule {
	string lhs;
	vector<string> rhs;
};

struct action {
	bool shift;
	int toState;
};
struct node {
	string symbol;
	vector<node> children;
};

typedef map<int, map<string, action> > rules;
typedef pair<string, string> deuce;

ifstream in("wlp4.lr1");
vector<string> terminals, nonterminals;
string start;
vector<prodRule> prodRules;
int statesCount;
rules dfa;
vector<deuce> toParse;

stack<int> parseStack;
vector<node> treeStack;

inline void cfg();
inline void lr1();
inline void parse();
node makeTree(string&);
node makeTree(string&, vector<node>);
node makeTree(string&, node);
void traverse(vector<node>&);

int main() {
	cfg();
	lr1();
	try {
		parse();
		cout << "start BOF procedures EOF" << endl;
		traverse(treeStack);
	}
	catch(string err) {
		cerr << err << endl;
	}
}


inline void cfg() {
	int count = 0;
	string s;
	string lhs;
	vector<string> rhs;

	in >> count;
	for(int i=0; i<count; ++i) {
		in >> s;
		terminals.push_back(s);
	}

	in >> count;
	for(int i=0; i<count; ++i) {
		in >> s;
		nonterminals.push_back(s);
	}

	in >> start >> count;

	getline(in, s);
	for(int i=0; i<count; ++i) {
		getline(in, s);

		stringstream ss(s);

		ss >> lhs;
		while(ss >> s) {
			rhs.push_back(s);
		}

		prodRule p = {lhs, rhs};

		prodRules.push_back(p);
		rhs.clear();
	} 
}

inline void lr1() {
	int t = 0;
	int state;
	string s, symbol;
	action a;

	in >> statesCount >> t;

	for(int i=0; i<t; ++i) {
		in >> state;
		in >> symbol;
		in >> s;
		a.shift = ((s == "shift") ? 1 : 0);
		in >> a.toState;

		dfa[state][symbol] = a;
	}
}

inline void parse() {
	string s, str2, str3;
	stringstream out;
	prodRule p;
	int state, s2;
	action a, a2;
	node tree;
	vector<node> children;

	toParse.push_back(make_pair(string("BOF"), string("BOF")));
	while(getline(cin, s)) {
		stringstream ss(s);
		ss >> str2 >> str3;
		toParse.push_back(make_pair(str2, str3));
	}
	toParse.push_back(make_pair(string("EOF"), string("EOF")));
	
	parseStack.push(-1);
	parseStack.push(dfa[0][toParse[0].first].toState);

	treeStack.push_back(makeTree(toParse[0].first, makeTree(toParse[0].second)));

	int i = 1;
	while(i < toParse.size()) {
		state = parseStack.top();
		a = dfa[state][toParse[i].first];
		if(a.toState == 0) {
			stringstream sss;
			sss << i;
			throw string("ERROR at " + sss.str());
		}

		if(a.shift) {
			parseStack.push(a.toState);

			treeStack.push_back(makeTree(toParse[i].first, makeTree(toParse[i].second)));

			++i;
		}
		else {
			p = prodRules[a.toState];

			children.clear();

			for(int j=0; j<p.rhs.size(); ++j) { 
				parseStack.pop();

				children.push_back(treeStack[treeStack.size()-1]);
				treeStack.pop_back();
			}

			state = parseStack.top();
			a = dfa[state][p.lhs];
			parseStack.push(a.toState);

			treeStack.push_back(makeTree(p.lhs, children));
		}
	}
}

node makeTree(string &symbol, vector<node> tree) {
	node ret = { symbol, tree };
	return ret;
}

node makeTree(string &symbol, node tree) {
	vector<node> vn;
	vn.push_back(tree);
	
	return makeTree(symbol, vn);
}

node makeTree(string &symbol) {
	vector<node> vn;
	return makeTree(symbol, vn);
}

void traverse(vector<node> &tree) {
	for(int i=tree.size()-1; i>=0; --i) {
		if(tree[i].children.empty() && find(nonterminals.begin(), nonterminals.end(), tree[i].symbol) == nonterminals.end()) {
			continue;
		}
		else if(tree[i].symbol == "BOF") {
			cout << "EOF EOF" << endl;
			continue;
		}
		else if(tree[i].symbol == "EOF") {
			cout << "BOF BOF" << endl;
			continue;
		}
		cout << tree[i].symbol;
		for(int j=tree[i].children.size()-1; j>=0; --j) {
			cout << " " << tree[i].children[j].symbol;
		}
		cout << endl;
		traverse(tree[i].children);
	}
}