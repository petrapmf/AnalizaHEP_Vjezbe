#include "ElementaryParticle.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;
using namespace EP;

int main() {
	//srand(time(NULL));

	random_device rd; // random seed
	mt19937 gen(rd()); // initialize pseudo-random number generator
	uniform_real_distribution<float> dist(-100.0, 100.0); // range 

	int i;
	ofstream MyFile;
	MyFile.open("Analysis.txt");

	MyFile << "1st NAME\tpx   py   pz\t\t\t2nd NAME\tpx   py   pz\n";

	for (i = 0; i < 10000; i++) {
		ElementaryParticle higgs("Higgs boson", 124.97, true);
		
		higgs.px = dist(gen);
		higgs.py = dist(gen);
		higgs.pz = dist(gen);
		//higgs.momentum(higgs.px, higgs.py, higgs.pz);
		//higgs.transMomentum(higgs.px, higgs.py);

		ElementaryParticle* particle1 = new ElementaryParticle();
		ElementaryParticle* particle2 = new ElementaryParticle();
		higgs.bosonDecay(particle1, particle2);

		MyFile << particle1->name << "   " << particle1->px << "   " << particle1->py << "   " << particle1->pz << "      " << particle2->name << "   " << particle2->px << "   " << particle2->py << "   " << particle2->pz << "\n";
	}

	MyFile.close();
	return 0;
}