#include <fstream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>

using namespace std;
 

class Analyzer 
{
public:

		Analyzer(); //konstruktor

		void readAnalysis(); //funkcije
		void ConvertTxtToRootFile();
};
