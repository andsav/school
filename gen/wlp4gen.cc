#include "Valid.h"
#include "Mips.h"

int main() {
	Tree parseTree = readTree();
	table symbols;
	vector<string> order;
	cell signature;
	try{
		symbols = genSymbols(parseTree);
		order = getOrder();
		signature = getSignature();
		testTypes(parseTree);

		Mini mini(parseTree, symbols, order);
		Split flow(mini);
		Opt instr(flow);
		Loc loc(instr);
		Mips m(instr, loc);

		m.print();
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
}
