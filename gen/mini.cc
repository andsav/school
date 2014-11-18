#include "Mini.h"

/*
*	INSTRUCTIONS
*/

Instr::Instr(string var, char cmd, string args)
	: var(var), cmd(cmd) { this->splitArgs(args); }

Instr::Instr(char cmd, string args) 
	: cmd(cmd) { this->splitArgs(args); }

Instr::Instr() : cmd(0) { }

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
		FOREACH(instr.args) {
			out << " " << instr.args[i];
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

	this->genFullSymbols();
}

Mini::~Mini() { }

void Mini::genCode(Tree &t) {
	if(t.lhs() == "procedure" || t.rhs()[0] == "main") {
		Tree *dcls, *statements, *ret;
		string returnTo;

		if(t.rhs()[0] == "main") {
			this->current 	= "wain";
			dcls 			= &t.children[0].children[8];
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
		APPEND(code, retCode);

		// statements
		retCode = this->statementsCode(*statements);
		APPEND(code, retCode);

		 // RETURN expr
		retCode = this->exprCode(returnTo, *ret);
		APPEND(code, retCode);

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

		string temp = makeTemp();

		vector<Instr> ret1, ret2;
		ret2 = this->exprCode(temp, t.children[0]);
		ret1 = this->exprCode(var, t.children[2]);

		
		APPEND(ret, ret1);
		APPEND(ret, ret2);

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

			APPEND(ret, ret2);
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
		else if(t.toString() == "statement lvalue BECOMES expr SEMI") {
			string var = this->getLvalue(t.children[0]);
			ret = this->exprCode(var, t.children[2]);
			/*
			ret.push_back(Instr(
				var,
				'=',
				string("$3")
				));
			*/
		}
		else if(t.rhs()[0] == "WHILE") {
			return this->loopCode(t);
		}
		else if(t.rhs()[0] == "IF") {
			return this->ifCode(t);
		}
	}

	return ret;
}

string Mini::getLvalue(Tree &t) {
	if(t.toString() == "lvalue ID") {
		return t.children[0].rhs()[0];
	}
	else if(t.toString() == "lvalue STAR factor") {
		// @TODO
		//
		//
		//
		//
	}
	else {	// lvalue LPAREN lvalue RPAREN
		return this->getLvalue(t.children[1]);
	}
}

string Mini::getDcl(Tree &t) {
	return t.children[1].rhs()[0];
}

vector<Instr> Mini::dclsCode(Tree& t){
	vector<Instr> ret;
	if(t.rhs().empty()) {
		return ret;
	}

	if(t.rhs()[3] == "NUM") {
		string var = this->getDcl(t.children[1]);

		ret = this->dclsCode(t.children[0]);
		ret.push_back(Instr(
			var,
			'=',
			t.children[3].rhs()[0]));
	}
	else if(t.rhs()[3] == "NULL") {
		// @TODO
		//
		//
		//

	}

	return ret;
}

vector<Instr> Mini::testCode(Tree& t, string& temp1, string& temp2) { // test expr X expr
	vector<Instr> ret, ret2;

	ret = this->exprCode(temp1, t.children[0]);
	ret2 = this->exprCode(temp2, t.children[2]);

	APPEND(ret, ret2);

	return ret;
}

vector<Instr> Mini::loopCode(Tree& t) {
	vector<Instr> ret, ret2;

	string temp1 = makeTemp();
	string temp2 = makeTemp();
	string testCode = string(temp1 + " " + t.children[2].children[1].rhs()[0] + " " + temp2);
	string id = this->makeLoop();

	ret.push_back(Instr(id, 'W'));

	ret2 = this->testCode(t.children[2], temp1, temp2);
	APPEND(ret, ret2);

	ret.push_back(Instr(
		id,
		'T',
		testCode));

	ret2 = this->statementsCode(t.children[5]);
	APPEND(ret, ret2);
	ret.push_back(Instr(
		id,
		'w'));

	return ret;
}

vector<Instr> Mini::ifCode(Tree& t) {
	vector<Instr> ret, ret2;

	string temp1 = makeTemp();
	string temp2 = makeTemp();
	string testCode = string(temp1 + " " + t.children[2].children[1].rhs()[0] + " " + temp2);
	string id = this->makeIf();

	ret = this->testCode(t.children[2], temp1, temp2);
	ret.push_back(Instr(
		id,
		'I',
		testCode));

	ret2 = this->statementsCode(t.children[5]);
	APPEND(ret, ret2);
	ret.push_back(Instr(
		id,
		'e'));

	ret2 = this->statementsCode(t.children[9]);
	APPEND(ret, ret2);

	ret.push_back(Instr(
		id,
		'i'));

	return ret;
}

procedures* Mini::getCode() {
	return &this->code;
}

void Mini::genFullSymbols() {
	for(table::iterator it = this->symbols.begin(); it != this->symbols.end(); ++it) {
		string current = it->first;

		for(cell::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			this->fullSymbols[current][it2->first] = it2->second;
		}

		for(int i=1; i <= this->tempCount[current]; ++i) {
			stringstream ss;
			ss << i;
			this->fullSymbols[current][string("t" + ss.str())] = "int";
		}

		//this->fullSymbols[current]["$3"] = "int";
		//this->fullSymbols[current]["$1"] = "int";
	}
}

table* Mini::getFullSymbols() {
	return &this->fullSymbols;
}

// SUGAAAR
string Mini::makeTemp() {
	this->tempCount[this->current]++;

	stringstream tc;
	tc << tempCount[this->current];

	return string("t" + tc.str());
}

string Mini::makeIf() {
	this->ifCount++;
	stringstream ic;
	ic << ifCount;

	return string("I" + ic.str());
}

string Mini::makeLoop() {
	this->loopCount++;
	stringstream ic;
	ic << loopCount;

	return string("W" + ic.str());
}

ostream& operator<<(ostream& out, Mini& mini) {
	FOREACH(mini.order) {
		out << mini.order[i] << ":" << endl;
		for(int j=0; j<mini.code[mini.order[i]].size(); ++j) {
			out << mini.code[mini.order[i]][j];
		}
	}
}