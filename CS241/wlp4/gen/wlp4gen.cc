#include "Mips.h"

int main() {
	try{

		Valid::genParseTree();

		//cout << parseTree;

		//throw string("STOP");

		Valid::genCode();

		/*
		FOREACH(program) {
			cout << endl << endl << program[i].name <<  ":" << endl;
			cout << program[i];
		}
		throw(string("die"));
		*/
		FOREACH(program) {
			current = &program[i];
			FOREACH_(current->symbols, j) {
				current->symbols[j]->use.second = current->instr.size()-1;
			}
		}

		Loc::genLoc();

		/*
		FOREACH(program) {
			current = &program[i];
			cout << current->name << endl << endl;
			FOREACH_(current->symbols, j) {
				cout << current->symbols[j]->name << " "
					 << current->symbols[j]->def.first << " - " << current->symbols[j]->use.second
					 << " ::" << current->symbols[j]->loc << endl;
			}
			FOREACH_(current->usedRegs, j) {
				cout << current->usedRegs[j] << endl;
			}
		}
		
		throw string("HERE");
	*/
		
		Mips::genCode();

		cout << code.rdbuf();

	}
	catch(string s) {
		cerr << "ERROR: " << s << endl;
	}
}

#include "Mips.h"