#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Analyzer.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>

using namespace std;

Analyzer::Analyzer() {

}

void Analyzer::readAnalysis() {
	ifstream MyFile;
	MyFile.open("/home/pakrap/Vjezbe/Vjezbe_3/Analysis.txt");

	string line, name1, name2;
	float px1, px2, py1, py2, pz1, pz2, E1, E2;
	bool skip;

	skip = true;

	if (MyFile.is_open()) {
		while (getline(MyFile, line)) {
			stringstream linestream(line);

			if (skip) {
				skip = false;
				continue;
			}
			linestream >> name1 >> px1 >> py1 >> pz1 >> E1 >> name2 >> px2 >> py2 >> pz2 >> E2;
			cout << name1 << " "<< px1 << " " << py1 << " " << pz1 << " " << E1 << " " << name2 << " " << px2 << " "<< py2 << " "<< pz2 << " "<< E2 << endl;
		}
	}
	MyFile.close();
}

void Analyzer::ConvertTxtToRootFile() {

	ifstream input;
	input.open("/home/pakrap/Vjezbe/Vjezbe_3/Analysis.txt");

	TFile* output = output = TFile::Open("Analysis.root", "RECREATE"); //TFile::Open(name, option);  
	TTree* tree = new TTree("tree", "Converting .txt to .root"); //TTree tree (name, title); Tree -> columnar dataset; TTree consists of a list of independent columns (branches, TBranch class)

	Char_t name1;
	Float_t px1;
	Float_t py1;
	Float_t pz1;
	Float_t E1;

	Char_t name2;
	Float_t px2;
	Float_t py2;
	Float_t pz2;
	Float_t E2;

	tree->Branch("name1", &name1, "name1/C"); //Branch(name, address, string describing the leaf list); leaf: name/type
	tree->Branch("px1", &px1, "px1/F");
	tree->Branch("py1", &py1, "py1/F");
	tree->Branch("pz1", &pz1, "pz1/F");
	tree->Branch("name2", &name2, "name2/C");
	tree->Branch("px2", &px2, "px2/F");
	tree->Branch("py2", &py2, "py2/F");
	tree->Branch("pz2", &pz2, "pz2/F");

	bool skip;
    skip = true;

	string line;
	if (input.is_open()) {
		while (getline(input, line)) {
			stringstream linestream(line);

			if (skip) {
				skip = false;
				continue;
			}
			linestream >> name1 >> px1 >> py1 >> pz1 >> E1 >> name2 >> px2 >> py2 >> pz2 >> E2;
			cout << name1 << " " << px1 << " " << py1 << " " << pz1 << " " << E1 << " " << name2 << " " << px2 << " " << py2 << " " << pz2 << " " << E2 << endl;
			tree->Fill();
		}
	}

	tree->Print();
	tree->Write();
	input.close();
	delete output;
}

