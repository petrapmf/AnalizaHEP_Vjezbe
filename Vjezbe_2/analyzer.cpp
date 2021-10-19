#include "ElementaryParticle.h"
#include <iostream>

using namespace EP;


int main() {
	ElementaryParticle higgs("Higgs boson", 124.97, true);
	ElementaryParticle top("Top quark", 173.1, false);
	ElementaryParticle* ptrZ = new ElementaryParticle("Z boson", 91.19, true);

	return 0;
}