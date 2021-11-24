#include <iostream>
#include <fstream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>
#include "train.h"
using namespace std;

int main() {
	train* a = new train();

	//a->readAnalysis();
	//a->ConvertTxtToRootFile();
	a->Plot();
	return 0;
}