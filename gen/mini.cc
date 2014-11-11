#include "Mini.h"

/*
*	INSTRUCTIONS
*/

Instr::Instr(string var, char cmd, string args)
	: var(var), cmd(cmd) { this->splitArgs(args); }

Instr::Instr(char cmd, string args) 
	: cmd(cmd) { this->splitArgs(args); }

Instr::Instr() { cmd = 0; }

void Instr::splitArgs(const string& args) {
	stringstream ss(args);
	string arg;
	while(ss >> arg) {
		this->args.push_back(arg);
	}
}

ostream& operator<<(ostream& out, const Instr& instr) {
	if(instr.cmd != 0) {
		out << instr.var << " " << instr.cmd;
		for(int j=0; j<instr.args.size(); ++j) {
			out << " " << instr.args[j];
		}
		out << endl;
	}
}

/*
*	MINI
*/

Mini::Mini(Tree& parseTree, table& symbols, vector<string>& order) : 
	parseTree(parseTree), symbols(symbols), order(order) {	
		this->genCode(this->parseTree.children[1]); // procedures procedure procedures
}

void Mini::genCode(Tree &t) {
	if(t.lhs() == "procedure" || t.rhs()[0] == "main") {
		Tree *dcls, *statements, *ret;
		string returnTo;

		if(t.rhs()[0] == "main") {
			this->current 	= "wain";
			dcls 			= &t.children[0].children[5];
			statements 		= &t.children[0].children[9];
			ret 			= &t.children[0].children[11];

			// dcl, dcl (wain args) assume they are $1, $2
			this->code["wain"].push_back(Instr(
				t.children[0].children[3].children[1].rhs()[0],
				'=',
				string("$1")
				));	

			this->code["wain"].push_back(Instr(
				t.children[0].children[5].children[1].rhs()[0],
				'=',
				string("$2")
				));

			 returnTo = "$3";
		}
		else {
			this->current 	= t.children[1].rhs()[0];
			dcls 			= &t.children[5];
			statements 		= &t.children[6];
			ret 			= &t.children[8];

			// argslist

			returnTo = "$5";
		}

		vector<Instr>& code = this->code[this->current];
		vector<Instr> retCode;

		// dcls
		retCode = this->dclsCode(*dcls);
		code.insert(code.end(), retCode.begin(), retCode.end());

		// statements
		retCode = this->statementsCode(*statements);
		code.insert(code.end(), retCode.begin(), retCode.end());

		 // RETURN expr
		retCode = this->exprCode(returnTo, *ret);
		code.insert(code.end(), retCode.begin(), retCode.end());

	}
	else {	// procedures procedure procedures
		this->genCode(t.children[0]);
		this->genCode(t.children[1]);
	}
}

vector<Instr> Mini::exprCode(string, Tree&) {
	vector<Instr> ret;
	return ret;
}
vector<Instr> Mini::statementsCode(Tree&) {
	vector<Instr> ret;
	return ret;
}
vector<Instr> Mini::dclsCode(Tree&){
	vector<Instr> ret;
	return ret;
}


ostream& operator<<(ostream& out, Mini& mini) {
	for(int i=0; i<mini.order.size(); ++i) {
		cout << mini.order[i] << ":" << endl;
		for(int j=0; j<mini.code[mini.order[i]].size(); ++j) {
			cout << mini.code[mini.order[i]][j];
		}
	}
}