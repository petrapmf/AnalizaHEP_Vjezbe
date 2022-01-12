#include <iostream>
#include <fstream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>
#include "Analyzer.h"
using namespace std;

int main() {
	Analyzer* a = new Analyzer();

	//cout << "CP Upper: " << a->UpperCPInterval(1, 10, 0.6827) << endl;
	//cout << "CP Lower: " << a->LowerCPInterval(1, 10, 0.6827) << endl;
	//a->ClopperPearson(10, 0.6827);
	a->PlotCPBelt(10, 0.6827);
	a->Dice(10, 0.6827);
	return 0;
}