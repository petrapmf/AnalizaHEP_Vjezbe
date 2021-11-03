#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>

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
    TH1F* h1 = new TH1F("h1", "Transversal momentum", 100, 0, 100);
    TH1F* h2 = new TH1F("h2", "Transversal momentum of particle 2", 100, 0, 100);

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

    }
    auto canvas = new TCanvas("canvas", "canvas", 600, 600);
    h1->Draw();
    h2->Draw("hist same");
    //h1->GetXaxis()->SetRangeUser(0, 100);
    h1->GetXaxis()->SetTitle("p_{T} [GeV]");
    h1->GetYaxis()->SetTitle("# of Events");
    h1->SetFillColor(kRed);
    h2->SetLineColor(kBlue);
    h1->SetLineColor(kRed);
    h1->SetStats(0);

    TLegend* legend = new TLegend(.7, .75, .89, .89); //geometrija
    //legend->SetHeader(h1->GetName());
    legend->AddEntry(h1, "Cestica 1", "f");
    legend->AddEntry(h2, "Cestica 2", "l");
    legend->Draw();
    //canvas->SaveAs("PT1.pdf");
    canvas->SaveAs("PT.pdf");
}