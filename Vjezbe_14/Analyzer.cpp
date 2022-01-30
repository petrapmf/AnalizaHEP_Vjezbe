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
#include <TRandom.h>

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

void Analyzer::Higgs() {
    float m_h, signal, bckg, pdf, p_value, chi;
    int dot = 0;
    auto cnvs = new TCanvas("canvas", "canvas", 1200, 800);
    TGraph* pvalue = new TGraph();
    h_chi->Scale(1.0 / h_chi->Integral());

    f->SetParameter(0, 200.0);
    f->SetParName(0, "N_{SM}");
    f->FixParameter(1, 100.0);
    f->SetParName(1, "#Zeta_{SM}");

    for (int i = 10; i <= 690; i = i + 5) {
        m_h = 1.0 * i; //skeniras pvalue kao funkciju razlicitih masa, od 10 do 680 GeVa
        signal = (-1.0) * (m_h - 190.0) * (m_h - 190.0) + 0.02; 
        for (int j = 0; j < 10000; j++) { //1 pseudoexp, 10^4 eventova
            if (r3->Rndm() > signal) {
                bckg = r3->Exp(100); 
                h_->Fill(bckg);
            }
            else {
                pdf = r3->Gaus(m_h, 0.0236 * m_h); //signal
                h_->Fill(pdf);
            }
        }
        h_->Fit(f, "Q", "", m_h - 10, m_h + 10);
        chi = f->GetChisquare(); //ovo je t observed!! sad za svaku masu trazis p_value
        p_value = h_chi->Integral(h_chi->FindBin(chi), 200);
        h_->Reset();
        pvalue->SetPoint(dot, m_h, p_value);

        dot++;
    }

    pvalue->GetXaxis()->SetTitle("m_{H} [GeV]");
    pvalue->GetYaxis()->SetTitle("p-value");
    pvalue->SetMinimum(0.00001);

    gPad->SetLogy();
    pvalue->Draw("AL*");

    //double z_score = TMath::Sqrt(2) * TMath::ErfcInverse(2 * 0.00135);
    //std::cout << "Significance: " << z_score << std::endl;
    cnvs->SaveAs("p-value.pdf");
}
