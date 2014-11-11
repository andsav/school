#include "Valid.h"
#include "Mini.h"

int main() {

	Tree parseTree = readTree();
	table symbols;
	vector<string> order;
	cell signature;

	try{
		symbols = genSymbols(parseTree);
		order = getOrder();
		signature = getSignature();
		//testTypes(parseTree);

		Mini m(parseTree, symbols, order);
		cout << m;
		//Split flow(mini.getCode());
		//Opt instr(flow);
		//Loc loc(mini.getCode());
		//Mips m(mini.getCode(), loc);

		//m.print();
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
	
}
