#define Height_cxx
#include "Height.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TMath.h>
#include <TLine.h>
#include <TLegend.h>
#include <iostream>

void Height::TestStatPDF()
{
//   In a ROOT session, you can do:
//      root> .L Height.C
//      root> Height t
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
   float suma = 0.0;
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      suma = suma + height;

   }
   float average = suma / nentries; //za dane podatke, t_observed

   TRandom* rand;
   rand = new TRandom();
   TH1F* h_teststat = new TH1F("h_teststat", "Test Statistic", 500, 160, 170);
   float sum, teststat;

   for (int j = 1; j <= 1000000; j++) { //pseudoexp
       sum = 0.0;
       for (int i = 1; i <= 100; i++) { //visine 100 zena
           sum = sum + rand->Gaus(164.7, 7.1);
       }
       teststat = sum / 100.0;
       h_teststat->Fill(teststat);
   }

   auto canvas = new TCanvas("canvas", "canvas", 1200, 800);

   float p_value;
   h_teststat->Scale(1. / h_teststat->Integral()); 
   p_value = h_teststat->Integral(h_teststat->FindBin(average), 500);
   std::cout << "p value = " << p_value << std::endl;
   if (p_value < 0.05)
       std::cout << "Rejected!" << std::endl;

   h_teststat->GetXaxis()->SetTitle("t");
   h_teststat->GetYaxis()->SetTitle("PDF");
   h_teststat->Draw("hist");

   float z_score = TMath::Sqrt(2) * TMath::ErfcInverse(2 * p_value);
   std::cout << "Significance: " << z_score << std::endl;

   auto l1 = new TLine(average, 0.0, average, 0.01);
   l1->SetLineStyle(kDashed);
   l1->Draw();
   canvas->SaveAs("Test Statistic.pdf");
}

void Height::HypothesisTesting(TString country, double mu, double sigma){

    if (fChain == 0) return;
    float suma = 0.0;
    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        suma = suma + height;

    }
    float average = suma / nentries; //za dane podatke, t_observed

    TRandom* r = new TRandom();
    TH1F* h0 = new TH1F("h0", "Hypothesis testing", 500, 160, 175); //null hypothesis --> Spain
    TH1F* h1 = new TH1F("h1", "Hypothesis testing", 500, 160, 175); //alt hypothesis
    double sum0, sum1, ts0, ts1, p_value0, p_value1; 
    double confidence_lvl;

    for (int i = 0; i < 1000000; i++) {
        sum0 = 0.0;
        sum1 = 0.0;
        for (int j = 0; j < 100; j++) {
            sum0 = sum0 + r->Gaus(168.0, 7.0); 
            sum1 = sum1 + r->Gaus(mu, sigma); 
        }
        ts0 = sum0 / 100.0;
        ts1 = sum1 / 100.0;
        h0->Fill(ts0);
        h1->Fill(ts1);
    }
    h0->Scale(1. / h0->Integral());
    h1->Scale(1. / h1->Integral());
    p_value0 = h0->Integral(h0->FindBin(average), 500);
    p_value1 = h1->Integral(h0->FindBin(average), 500);
    std::cout << "p value for H0 = " << p_value0 << std::endl;
    std::cout << "p value for H1 = " << p_value1 << std::endl;

    if (country == "NL") {
        confidence_lvl = 1.0 - (h1->Integral(0, h1->FindBin(average)) / p_value0);
    }
    else {
        confidence_lvl = 1.0 - (p_value1 / h0->Integral(0, h0->FindBin(average)));
    }

    std::cout << "Rejecting " << country << " with confidence level of " << confidence_lvl * 100 << "%" << std::endl;

    auto c = new TCanvas("c", "c", 1200, 800);

    h0->SetLineColor(kRed);
    h0->GetXaxis()->SetTitle("t");
    h0->GetYaxis()->SetTitle("PDF");
    h1->SetAxisRange(0.0, 0.02, "Y");
    h0->Draw("hist");
    h1->SetLineColor(kBlue);
    h1->GetXaxis()->SetTitle("t");
    h1->GetYaxis()->SetTitle("PDF");
    h1->Draw("hist same");

    TLegend* l = new TLegend(.1, .7, .3, .9);
    l->SetHeader("Legend", "C");
    l->AddEntry(h0, "ES", "l");
    l->AddEntry(h1, country, "l");
    l->Draw();

    TLine* line = new TLine(average, 0.0, average, 0.017);
    line->Draw();

    c->SaveAs("ES&" + country + ".pdf");
}

