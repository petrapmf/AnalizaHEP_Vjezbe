#define Analyzer_cxx
#include "Analyzer.h"
#include <cmath>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLorentzVector.h>

void Analyzer::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Analyzer.C
//      root> Analyzer t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      std::cout << px1 << std::endl;
      // if (Cut(ientry) < 0) continue;
   }
}

void Analyzer::PlotHistogram() 
{
    TH1F* h1 = new TH1F("h1", "Transversal momentum", 100, 0, 150);
    TH1F* h2 = new TH1F("h2", "Transversal momentum of particle 2", 100, 0, 150);
    TH1F* h3 = new TH1F("h3", "Transversal momentum of Higgs boson", 100, 0, 150);

    TLorentzVector* p1 = new TLorentzVector();
    TLorentzVector* p2 = new TLorentzVector();
    TLorentzVector* higgs = new TLorentzVector();
    
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        //std::cout << px1 << std::endl;

        h1->Fill(pt1);
        h2->Fill(pt2);

        p1->SetPxPyPzE(px1, py1, pz1, E1);
        p2->SetPxPyPzE(px2, py2, pz2, E2);
        //std::cout << p1->Px() << " " << p2->Px() << std::endl;

        //higgs->SetPxPyPzE(px1 + px2, py1 + py2, pz1 + pz2, E1 + E2);
        *higgs = *p1 + *p2;
        //std::cout << higgs->Px() << "!" << std::endl;

        h3->Fill(higgs->Pt());
        h3->Fill(sqrt(pow(higgs->Px(), 2.) + pow(higgs->Py(), 2.)));
    }
    auto canvas = new TCanvas("canvas", "canvas", 1000, 500);
    canvas->Divide(2, 2);
    
    canvas->cd(1);
    h1->Draw();

    canvas->cd(1);
    h2->Draw("hist same");
    
    canvas->cd(2); //change pad
    h3->Draw("hist same");

    gStyle->SetOptStat(0);
    //h1->GetXaxis()->SetRangeUser(0, 100);
    h1->GetXaxis()->SetTitle("p_{T} [GeV]");
    h3->GetXaxis()->SetTitle("p_{T} [GeV]");
    h1->GetYaxis()->SetTitle("# of Events");
    h3->GetYaxis()->SetTitle("# of Events");

    h1->SetFillColor(kRed);
    h2->SetLineColor(kBlue);
    h1->SetLineColor(kRed);
    h1->SetStats(0);
    h3->SetLineColor(kCyan);
    h3->SetFillColor(kCyan);

    TLegend* legend = new TLegend(.7, .75, .89, .89); //geometrija
    //legend->SetHeader(h1->GetName());
    legend->AddEntry(h1, "Particle 1", "f");
    legend->AddEntry(h2, "Particle 2", "l");
    legend->AddEntry(h3, "Higgs boson", "f");
    legend->Draw();

    /*canvas->SaveAs("PT.pdf");
    canvas->SaveAs("PT1.png");
    canvas->SaveAs("PT1.pdf");
    canvas->SaveAs("PT1.root");*/

   canvas->SaveAs("Higgs.png");
   canvas->SaveAs("Higgs.pdf");
   canvas->SaveAs("Higgs.root");
}