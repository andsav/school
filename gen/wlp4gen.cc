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

		Mini m(parseTree, symbols, order);
		// FUN TIMES
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
}
