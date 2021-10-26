#include "ElementaryParticle.h"
#include <iostream>

using namespace EP;


int main() {
	ElementaryParticle higgs("Higgs boson", 124.97, true);
	//ElementaryParticle top("Top quark", 173.1, false);
	//ElementaryParticle* ptrZ = new ElementaryParticle("Z boson", 91.19, true);
	higgs.px = 2.0;
	higgs.py = 1.0;
	higgs.pz = 2.0;
	higgs.momentum(higgs.px, higgs.py, higgs.pz);
    higgs.transMomentum(higgs.px, higgs.py);

	ElementaryParticle* particle1 = new ElementaryParticle();
	ElementaryParticle* particle2 = new ElementaryParticle();
	higgs.bosonDecay(particle1, particle2);
	//top.bosonDecay(particle1, particle2);

	return 0;
}