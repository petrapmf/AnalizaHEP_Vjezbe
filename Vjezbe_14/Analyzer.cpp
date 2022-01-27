#define Analyzer_cxx
#include "Analyzer.h"
#include <TH1F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cmath>
#include <TGraphErrors.h>
#include <TLine.h>
#include <TColor.h>
#include <TRandom3.h>

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
      // if (Cut(ientry) < 0) continue;
   }
}

void Analyzer::TestStat() {
    float pdf, t_chi, m;

    f->SetParameter(0, 200.0);
    f->SetParName(0, "N_{SM}");
    f->FixParameter(1, 100.0);
    f->SetParName(1, "#Zeta_{SM}");

    for (int i = 0; i < 1000; i++) { //pseudoexp
        for (int j = 0; j < 10000; j++) { //events
            pdf = r3->Exp(100.0);
            h->Fill(pdf);
        }
        for (int k = 10; k <= 690; k = k + 5) { //fitanje u "prozorima", tj. u intervalima od 20 GeV-a, preko cijele distribucije
            m = 1. * k;
            h->Fit(f, "Q", "", m - 10, m + 10);
            t_chi = f->GetChisquare(); // chi^2 je test statistika
            h_chi->Fill(t_chi);
        }
    }
    auto c = new TCanvas("c", "c", 1200, 800);

    h_chi->SetLineColor(kRed);
    h_chi->GetXaxis()->SetTitle("t");
    h_chi->GetYaxis()->SetTitle("PDF");
    h_chi->Draw();

    c->SaveAs("Chi.pdf");
}
