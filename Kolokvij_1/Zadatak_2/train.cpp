#define train_cxx
#include "train.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TColor.h>

void train::Loop()
{
//   In a ROOT session, you can do:
//      root> .L train.C
//      root> train t
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

void train::Plot() {
    TH1F* hpt = new TH1F("hpt", "Electron Transversal Momentum", 100, 0, 100);
    TH2F* h2D = new TH2F("2D", "Electron Transversal Momentum & Eta", 100, 0, 100, 80, 0, 5);
    TH1F* h1 = new TH1F("h1", "Ele_ep & Ele_eelepout", 50, -1, 8);
    TH1F* h2 = new TH1F("h2", "Ele_eelepout", 70, -1, 8);

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < 100000; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        hpt->Fill(ele_pt);
        h2D->Fill(ele_pt, scl_eta);
        h1->Fill(ele_ep);
        h2->Fill(ele_eelepout);
    }

    auto canvas = new TCanvas("canvas", "canvas", 1500, 300);
    canvas->Divide(3, 1);

    gStyle->SetOptStat(0);
    canvas->cd(1);
    hpt->GetXaxis()->SetTitle("p_{T} [GeV]");
    hpt->GetYaxis()->SetTitle("# of Events");
    hpt->SetLineColor(kRed);
    hpt->Draw("hist");

    canvas->cd(2);
    h2D->GetXaxis()->SetTitle("p_{T} [GeV]");
    h2D->GetYaxis()->SetTitle("Eta");
    h2D->SetMinimum(-0.01);
    h2D->Draw("COLZ");

    canvas->cd(3);
    h1->Scale(1.0 / h1->Integral());
    h2->Scale(1.0 / h2->Integral());
    h1->SetLineColor(kRed);
    h1->SetLineStyle(kDashed);
    h1->Draw("hist");

    h2->SetLineColor(kBlue);
    h2->SetFillColor(kBlue);
    h2->Draw("hist same");

    TLegend* l1 = new TLegend(.7, .75, .89, .89);
    l1->AddEntry(h1, "Ele_ep", "l");
    l1->AddEntry(h2, "Ele_eelepout", "f");
    l1->Draw();
    canvas->SaveAs("Electron.pdf");
}
