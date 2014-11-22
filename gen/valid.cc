#include "Valid.h"

Tree parseTree;
int tempCount = 0;
int ifCount = 0;
int loopCount = 0;
int offset = 0;;

char Valid::operation(const string &s) {
	if(s == "==")
		return '=';
	else if(s == "!=")
		return 'N';
	else if(s == "<")
		return '<';
	else if(s == ">")
		return '>';
	else if(s == "<=")
		return 'L';
	else if(s == ">=")
		return 'G';
}

bool Valid::isTerminal(const string &s) {
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

void Valid::genParseTree() {
	string s;
	getline(cin, s);

	parseTree = Tree(s);

	if(!isTerminal(parseTree.lhs)) {
		FOREACH(parseTree.rhs) {
			parseTree.children.push_back(readTree());
		}
	}
}

Tree* Valid::readTree() {
	string s;

	getline(cin, s);
	Tree* ret = new Tree(s);

	if(!isTerminal(ret->lhs)) {
		FOREACH(ret->rhs) {
			ret->children.push_back(readTree());
		}
	}
	else if(ret->lhs == "ID") {
		ret->rhs[0] = string("v" + ret->rhs[0]);
	}

	return ret;
}

void Valid::genCode(Tree* t) {
	if(t->lhs == "procedure" || t->rhs[0] == "main") {
		Tree *dcls, *statements, *ret;
		string returnTo;

		if(t->rhs[0] == "main") {
			program.push_front(Procedure(string("wain")));
			current = &program.front();

			dcls 			= t->children[0]->children[8];
			statements 		= t->children[0]->children[9];
			ret 			= t->children[0]->children[11];
			
			string& arg1 = t->children[0]->children[3]->children[1]->rhs[0];
			string& arg2 = t->children[0]->children[5]->children[1]->rhs[0];

			bool arg1Type = (*t->children[0]->children[3]->children[0] == "type INT") ? 1 : 0;

			current->addSymbol(new Symbol(arg1, arg1Type));
			current->addSymbol(new Symbol(arg2, 1));

			current->addInstr(new Instr(arg1, '=', string("$1")));
			current->addInstr(new Instr(arg2, '=', string("$2")), current->instr.back());

			// TODO: MIPS array
			//
			//

			returnTo = "$3";
		}
		else {
			program.push_front(Procedure(t->children[1]->rhs[0]));
			current = &program.front();

			dcls 			= t->children[5];
			statements 		= t->children[6];
			ret 			= t->children[8];

			// TODO

			returnTo = "$5";
		}

		dclsCode(dcls);
		statementsCode(statements);
		exprCode(returnTo, ret);

	}
	else {	// procedures procedure procedures
		genCode(t->children[0]);
		genCode(t->children[1]);
	}
}

void Valid::dclsCode(Tree* t) {
	if(t->rhs.empty()) {
		return;
	}

	if(t->rhs[3] == "NUM" || t->rhs[3] == "NULL") {
		string var = t->children[1]->children[1]->rhs[0];

		dclsCode(t->children[0]);
		bool isNum = ((t->rhs[3] == "NUM") ? 1 : 0);

		current->addSymbol(new Symbol(var, isNum));
		current->addInstr(new Instr(var, '=', t->children[3]->rhs[0]), current->instr.back());
	}
}

void Valid::statementsCode(Tree *t) {
	if(t->lhs == "statements" && !t->rhs.empty()) { // *t == "statements statements statement"
		statementsCode(t->children[0]);
		statementsCode(t->children[1]);
	}
	else if(t->lhs == "statement") {
		if(t->rhs[0] == "PRINTLN") { // PRINTLN LPAREN expr RPAREN SEMI
			exprCode(string("$1"), t->children[2]);
			current->addInstr(new Instr(string("$1"), 'P'), current->instr.back());
		}
		else if(t->rhs[0] == "lvalue") { // lvalue BECOMES expr SEMI
			Args lv = getLvalue(t->children[0]);
			if(lv.cmd == '@') {
				string temp = makeTemp();
				exprCode(temp, t->children[2]);
				current->addInstr(new Instr(lv.var1, '@', temp), current->instr.back());
			}
			else {
				exprCode(lv.var1, t->children[2]);
			}
		}
		else if(t->rhs[0] == "WHILE") { // WHILE LPAREN test RPAREN LBRACE statements RBRACE 
			loopCode(t);
		}
		else if(t->rhs[0] == "IF") { // IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE 
			ifCode(t);
		}
	}
}

void Valid::testCode(Tree* t, string& temp1, string& temp2) {
	exprCode(temp1, t->children[0]);
	exprCode(temp2, t->children[2]);
}

void Valid::ifCode(Tree* t) { // IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE 
	string temp1 = makeTemp();
	string temp2 = makeTemp();
	Instr *origin, *ifBlock, *elseBlock;

	Args test = Args(temp1, operation(t->children[2]->children[1]->rhs[0]), temp2);

	testCode(t->children[2], temp1, temp2);
	string id = makeIf();
	origin = current->instr.back();

	current->addInstr(new Instr(id, 'I', test), origin);

	statementsCode(t->children[5]);
	ifBlock = current->instr.back();

	current->addInstr(new Instr(id, 'e'), origin);

	statementsCode(t->children[9]);
	elseBlock = current->instr.back();	

	current->addInstr(new Instr(id, 'i'), ifBlock, elseBlock);
}

void Valid::loopCode(Tree* t) { // WHILE LPAREN test RPAREN LBRACE statements RBRACE 
	string temp1 = makeTemp();
	string temp2 = makeTemp();
	Instr *origin, *testBlock, *loopBlock;

	Args test = Args(temp1, operation(t->children[2]->children[1]->rhs[0]), temp2);

	string id = makeLoop();
	origin = current->instr.back();

	current->addInstr(new Instr(id, 'W'), origin);
	
	testCode(t->children[2], temp1, temp2);
	testBlock = current->instr.back();

	current->addInstr(new Instr(id, 'T', test), testBlock);

	statementsCode(t->children[5]);
	loopBlock = current->instr.back();

	current->addInstr(new Instr(id, 'w'), testBlock);
	testBlock->in.push_back(loopBlock);
}

void Valid::lvalueCode(const string& var, Tree *t) {
	if(t->rhs[0] == "ID") {
		current->addInstr(new Instr(var, '=', t->children[0]->rhs[0]), current->instr.back()); 
	}
	else if(t->rhs[0] == "STAR") { // lvalue STAR factor
		starFactorCode(var, t->children[1]);
	}
	else { // LPAREN lvalue RPAREN
		lvalueCode(var, t->children[1]);
	}
}

void Valid::ampLvalueCode(const string& var, Tree *t) {
	if(t->rhs[0] == "ID") {
		current->addInstr(new Instr(var, '=', Args(t->children[0]->rhs[0], '&')), current->instr.back()); 
	}
	else if(t->rhs[0] == "STAR") { // lvalue STAR factor
		exprCode(var, t->children[1]);
	}
	else { // LPAREN lvalue RPAREN
		ampLvalueCode(var, t->children[1]);
	}
}

void Valid::starFactorCode(const string& var, Tree *t) {
	if(t->rhs[0] == "LPAREN") {  // factor LPAREN expr RPAREN
		string temp = makeTemp();
		exprCode(temp, t->children[1]);
		current->addInstr(new Instr(var, '=', Args(temp, '@')), current->instr.back()); 
	}
	else if(t->rhs[0] == "ID") {
		current->addInstr(new Instr(var, '=', Args(t->children[0]->rhs[0], '@')), current->instr.back()); 
	}
	else if(t->rhs[0] == "AMP") { // factor AMP lvalue
		lvalueCode(var, t->children[1]);
	}
	else {
		throw string("Yo dawg, I heard you like pointers");
	}
}


Args Valid::getLvalue(Tree *t) {
	if(t->rhs[0] == "ID") {
		return Args(t->children[0]->rhs[0]); 
	}
	else if(t->rhs[0] == "STAR") { // lvalue STAR factor
		return getStarFactor(t->children[1]);
	}
	else { // LPAREN lvalue RPAREN
		return getLvalue(t->children[1]);
	}
}

Args Valid::getAmpLvalue(Tree *t) {
	if(t->rhs[0] == "ID") {
		return Args(t->children[0]->rhs[0], '&'); 
	}
	else if(t->rhs[0] == "STAR") { // lvalue STAR factor
		return getFactor(t->children[1]);
	}
	else { // LPAREN lvalue RPAREN
		return getAmpLvalue(t->children[1]);
	}
}

Args Valid::getFactor(Tree *t) {
	if(t->rhs[0] == "LPAREN") {  // factor LPAREN expr RPAREN
		string temp = makeTemp();
		exprCode(temp, t->children[1]);
		return Args(temp);
	}
	else if(t->rhs[0] == "NULL") { // factor NULL
		string temp = "NULL";
		return Args(temp);
	}
	else if(t->rhs[0] == "ID" || t->rhs[0] == "NUM") {
		return Args(t->children[0]->rhs[0]);
	}
	else if(t->rhs[0] == "AMP") { // factor AMP lvalue
		return getAmpLvalue(t->children[1]);
	}
	else if(t->rhs[0] == "STAR") { // factor STAR factor
		return getStarFactor(t->children[1]);
	}
}

Args Valid::getStarFactor(Tree *t) {
	if(t->rhs[0] == "LPAREN") {  // factor LPAREN expr RPAREN
		string temp = makeTemp();
		exprCode(temp, t->children[1]);
		return Args(temp, '@'); 
	}
	else if(t->rhs[0] == "ID") {
		return Args(t->children[0]->rhs[0], '@'); 
	}
	else if(t->rhs[0] == "AMP") { // factor AMP lvalue
		return getLvalue(t->children[1]);
	}
	else {
		throw string("Yo dawg, I heard you like pointers");
	}
}

void Valid::exprCode(const string& var, Tree *t) {
	if(*t == "expr term" || *t == "term factor") {
		exprCode(var, t->children[0]);
	}
	else if(t->lhs =="factor") {
		if(t->rhs[0] == "LPAREN") {  // factor LPAREN expr RPAREN
			exprCode(var, t->children[1]);
		}
		else if(t->rhs[0] == "NULL") { // factor NULL
			current->addInstr(new Instr(var, '=', string("NULL")), current->instr.back());
		}
		else if(t->rhs[0] == "ID" || t->rhs[0] == "NUM") {
			current->addInstr(new Instr(var, '=', t->children[0]->rhs[0]), current->instr.back()); 
		}
		else if(t->rhs[0] == "AMP") { // factor AMP lvalue
			ampLvalueCode(var, t->children[1]);
		}
		else if(t->rhs[0] == "STAR") { // factor STAR factor
			starFactorCode(var, t->children[1]);
		}
	}
	else if ((t->lhs == "expr" || t->lhs == "term") && t->rhs.size() == 3) {
		char operation;

		if(*t == "expr expr PLUS term")
			operation = '+';
		else if(*t == "expr expr MINUS term")
			operation = '-';
		else if(*t == "term term STAR factor")
			operation = '*';
		else if(*t == "term term SLASH factor")
			operation = '/';
		else if(*t == "term term PCT factor")
			operation = '%';

		string temp = makeTemp();

		exprCode(temp, t->children[0]);
		exprCode(var, t->children[2]);

		current->addInstr(new Instr(var, '=', Args(temp, operation, var)), current->instr.back());
	}
	else
		throw string("You shouldn't be here");
}

void Valid::genCode() {
	genCode(parseTree.children[1]); // procedures procedure procedures
}

string Valid::makeIf() {
	ifCount++;
	stringstream ic;
	ic << ifCount;

	return string("0I" + ic.str());
}

string Valid::makeLoop() {
	loopCount++;
	stringstream ic;
	ic << loopCount;

	return string("0W" + ic.str());
}

string Valid::makeTemp() {
	++tempCount;

	stringstream tc;
	tc << tempCount;

	string t = string("t" + tc.str());

	current->addSymbol(new Symbol(t, 1));

	return t;
}

string Valid::makePTemp() {
	++tempCount;

	stringstream tc;
	tc << tempCount;

	string t = string("t" + tc.str());

	current->addSymbol(new Symbol(t, 0));

	return t;
}