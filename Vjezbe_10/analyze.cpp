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

	a->PlotChi();
	return 0;
}