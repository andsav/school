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

bool Instr::empty() {
	return (this->cmd == 0);
} 

ostream& operator<<(ostream& out, Instr& instr) {
	if(!instr.empty()) {
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
			// Assumes MIPS twoints
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

			// TODO: MIPS array
			//
			//

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

vector<Instr> Mini::exprCode(string var, Tree& t) {
	vector<Instr> ret;

	if(t.toString() == "expr term" || t.toString() == "term factor") {
		return this->exprCode(var, t.children[0]);
	}
	else if(t.toString() == "factor LPAREN expr RPAREN") {
		return this->exprCode(var, t.children[1]);
	}
	else if(t.toString() == "factor ID" || t.toString() == "factor NUM") {
		ret.push_back(Instr(
			var,
			'=',
			t.children[0].rhs()[0]
			)); 
	}
	else if ((t.lhs() == "expr" || t.lhs() == "term") && t.rhs().size() == 3) {
		char operation;

		if(t.toString() == "expr expr PLUS term")
			operation = '+';
		else if(t.toString() == "expr expr MINUS term")
			operation = '-';
		else if(t.toString() == "term term STAR factor")
			operation = '*';
		else if(t.toString() == "term term SLASH factor")
			operation = '/';
		else if(t.toString() == "term term PCT factor")
			operation = '%';

		this->tempCount[this->current]++;

		stringstream tc;
		tc << tempCount[this->current];

		string temp = string("t" + tc.str());

		vector<Instr> ret1, ret2;
		ret2 = this->exprCode(temp, t.children[0]);
		ret1 = this->exprCode(var, t.children[2]);

		ret.insert(ret.end(), ret1.begin(), ret1.end());
		ret.insert(ret.end(), ret2.begin(), ret2.end());

		ret.push_back(Instr(
			var,
			'=',
			string(temp + " " + operation + " " + var)
			));
	}

	return ret;
}
vector<Instr> Mini::statementsCode(Tree& t) {
	vector<Instr> ret, ret2;
	if(t.lhs() == "statements") {
		if(t.rhs().empty()) {
			return ret;
		}
		else if(t.toString() == "statements statements statement") {
			ret = this->statementsCode(t.children[0]);
			ret2 = this->statementsCode(t.children[1]);

			ret.insert(ret.end(), ret2.begin(), ret2.end());
		}
	}
	else if(t.lhs() == "statement") {
		if(t.toString() == "statement PRINTLN LPAREN expr RPAREN SEMI") {
			ret = this->exprCode(string("$1"), t.children[2]);
			ret.push_back(Instr(
				string("$1"),
				'P'
				));
		}
	}

	return ret;
}
vector<Instr> Mini::dclsCode(Tree& t){
	vector<Instr> ret;
	return ret;
}

procedures Mini::getCode() {
	return this->code;
}

ostream& operator<<(ostream& out, Mini& mini) {
	for(int i=0; i<mini.order.size(); ++i) {
		cout << mini.order[i] << ":" << endl;
		for(int j=0; j<mini.code[mini.order[i]].size(); ++j) {
			cout << mini.code[mini.order[i]][j];
		}
	}
}