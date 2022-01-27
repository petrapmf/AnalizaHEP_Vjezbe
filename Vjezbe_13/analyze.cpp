#include <iostream>
#include <fstream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>
#include "Height.h"
using namespace std;

int main() {
	Height* a = new Height();
	//a->TestStatPDF();
	a->HypothesisTesting("FR", 164.7, 7.1); //France - FR; Italy - ITA; Netherlands - NL
	return 0;
}