#include "Def.h"

deque<Procedure> program;
Procedure* current;

/*
*	State
*/

State::State() : live(0) { }
State::State(stateType t) : live(0), t(t) { }
State::State(stateType t, string val) : live(0), t(t), val(val) { }

State State::lub(const State& s2) {
	switch(this->t) {
		case TOP:
			return *this;
		break;
		case K:
			if((s2.t == K && this->val == s2.val) || s2.t == BOT)
				return *this;
			else
				return State(TOP);
		break;
		default:
			return s2;
		break;
	}
}

bool State::operator==(const State& s2) const {
	return (this->t == s2.t && this->val == s2.val);
}
bool State::operator!=(const State& s2) const {
	return (this->t != s2.t || this->val != s2.val);
}

/*
*	SYMBOL
*/

Symbol::Symbol(string name) : name(name) { }
Symbol::Symbol(string name, string type) : name(name) {
	this->isInt = (type == "int") ? 1 : 0;
}
Symbol::Symbol(string name, bool isInt) : name(name), isInt(isInt) { }

/*
*	ARGS
*/
Args::Args() : var1(string()), cmd(0), var2(string()) { }
Args::Args(string var1, char cmd, string var2) : var1(var1), cmd(cmd), var2(var2) { }
Args::Args(string& var1, char cmd) : var1(var1), cmd(cmd) { }
Args::Args(string& s) : cmd(0) {
	stringstream ss(s);

	ss >> this->var1;

	if(ss.rdbuf()->in_avail() == 2) {
		ss >> this->cmd;
		ss >> this->var2;
	}
}
int Args::size() {
	int ret = 0;
	if(!this->var1.empty())
		++ret;
	if(this->cmd != 0)
		++ret;
	if(!this->var2.empty())
		++ret;


	return ret;
}

ostream& operator<<(ostream& out, Args& args) {
	if(!args.var1.empty()) {
		out << " " << args.var1;
	}
	if(args.cmd) {
		out << " " << args.cmd;
	}
	if(!args.var2.empty()) {
		out << " " << args.var2;
	}
}

/*
*	INSTRUCTION
*/
Instr::Instr(string var, char cmd, string args)
	: var(var), cmd(cmd) { 

	this->args = Args(args);
}

Instr::Instr(char cmd, string args) 
	: cmd(cmd) { 

	this->args = Args(args);
}

Instr::Instr(char cmd, Args args) : cmd(cmd), args(args) { }

Instr::Instr(string var, char cmd, Args args) : var(var), cmd(cmd), args(args) { }

Instr::Instr() : cmd(0) { 
	this->args = Args();
}

bool Instr::empty() {
	return (this->cmd == 0);
} 

void Instr::del() {
	this->cmd = 0;
}

ostream& operator<<(ostream& out, Instr& instr) {
	if(!instr.empty()) {
		out << instr.var << " " << instr.cmd;
		out << instr.args;
		out << endl;
	}
}

/*
*	PROCEDURE
*/
Procedure::Procedure() { }
Procedure::Procedure(string name) : name(name) { }
Procedure::~Procedure() {
	this->symbolsTable.clear();

	FOREACH(this->symbols)
		delete this->symbols[i];

	FOREACH(this->instr)
		delete this->instr[i];
}

void Procedure::addInstr(Instr* instr) {
	instr->proc = this;
	this->instr.push_back(instr);

	if(isVar(instr->var)) {
		this->symbolsTable[instr->var]->def.second = this->instr.size();
	}
	if(isVar(instr->args.var1)) {
		if(this->symbolsTable[instr->args.var1]->use.first == 0)
			this->symbolsTable[instr->args.var1]->use.first = this->instr.size();
		this->symbolsTable[instr->args.var1]->use.second = this->instr.size();
	} 
	if(isVar(instr->args.var2)) {
		if(this->symbolsTable[instr->args.var2]->use.first == 0)
			this->symbolsTable[instr->args.var2]->use.first = this->instr.size();
		this->symbolsTable[instr->args.var2]->use.second = this->instr.size();
	}
}

void Procedure::addInstr(Instr* instr, Instr* origin) {
	this->addInstr(instr);

	origin->out.push_back(instr);
	instr->in.push_back(origin);
}

void Procedure::addInstr(Instr* instr, Instr* origin1, Instr* origin2) {
	this->addInstr(instr, origin1);

	origin2->out.push_back(instr);
	instr->in.push_back(origin2);
}

void Procedure::addSymbol(Symbol* symbol) {
	symbol->proc = this;
	symbol->def = make_pair(this->instr.size(), this->instr.size());
	symbol->use = make_pair(0, 0);

	this->symbols.push_back(symbol);
	this->symbolsTable[symbol->name] = symbol;
	this->symbolsList.push_back(symbol->name);
}

ostream& operator<<(ostream& out, Procedure& proc) {
	FOREACH(proc.instr) {
		out << *(proc.instr[i]);
	}
}

/*
*	SORTSYMBOL
*/
sortSymbol::sortSymbol(Procedure* p, sortType sort) : p(p), sort(sort) { }

bool sortSymbol::operator() (const string& a, const string& b) {
	switch(sort) {
		case FIRST_DEF :
			return (p->symbolsTable[a]->def.first < p->symbolsTable[b]->def.first);
		break;
		case LAST_DEF :
			return (p->symbolsTable[a]->def.second < p->symbolsTable[b]->def.second);
		break;
		case FIRST_USE :
			return (p->symbolsTable[a]->use.first < p->symbolsTable[b]->use.first);
		break;
		case LAST_USE:
			return (p->symbolsTable[a]->use.second < p->symbolsTable[b]->use.second);
		break;
	}
}

/*
*	TREE
*/
Tree::Tree() { }

Tree::Tree(string& s) {
	this->init(s);
}

Tree::Tree(string& s, Tree* t) {
	this->init(s);
	this->children.push_back(t);
}

Tree::Tree(string& s, vector<Tree*> ts) {
	this->init(s);
	this->children = ts;
}

void Tree::init(string& s) {
	stringstream ss(s);
	string word;

	ss >> this->lhs;

	while(ss >> word) {
		this->rhs.push_back(word);
	}
}

Tree::~Tree() {
	FOREACH(this->children) {
		delete this->children[i];
	}
}

string Tree::toString() {
	string ret = this->lhs;
	
	FOREACH(this->rhs) {
		ret += string(" ") + this->rhs[i];
	}

	return ret;
}

void Tree::printTree(ostream& out, int level) {
	for(int i=0; i<level; ++i) {
		out << " ";
	}
	out << this->toString() << endl;
	FOREACH(this->children) {
		this->children[i]->printTree(out, level+1);
	}
}

bool Tree::operator==(const char* s) const {
	stringstream ss(s);
	string str;

	ss >> str;

	if(this->lhs != str)
		return false;

	FOREACH(this->rhs) {
		if(ss.rdbuf()->in_avail() == 0) {
			return false;
		}
		ss >> str;
		if(this->rhs[i] != str)
			return false;
	}

	if(ss.rdbuf()->in_avail() != 0) {
		return false;
	}
	
	return true;
}

bool Tree::operator!=(const char* s) const {
	return !(*this == s);
}

ostream& operator<<(ostream& out, Tree& t) {
	t.printTree(out, 0);
}


bool isNum(const string& s) {
	if(s.empty())
		return false;

	if(s[0] == '-') 
		return true;

    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool isReg(const string& s) {
	if(s.empty())
		return false;
	return (s[0] == '$');
}

bool isVar(const string& s) {
	if(s.empty())
		return false;
	if(s == "NULL")
		return false;
	return (s[0] != '0' && !isReg(s) && !isNum(s));
}
