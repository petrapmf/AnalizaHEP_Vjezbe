#define Analyzer_cxx
#include "Analyzer.h"
#include <TH1F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cmath>

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

void Analyzer::PlotHist() {
    TH1F* h = new TH1F("h", "Distribution", 50, 0, 10);
    TF1* fDecay = new TF1("fDecay", "[0]*(1.0/[1])*exp((-1.0/[1])*x)", 0.0, 20.0);
    TF1* fLikelihood = new TF1("fLikelihood", "195.2*(1.0/x)*exp((-1.0/x)*1.0)", 0.0, 20.0);
    TF1* fLN = new TF1("fLN", "2.0*([0]*log(x) + (1/x)*[1])", 1.0, 1.4);

    fDecay->SetParameter(0, 195.2); //iz 100 u 195.2
    fDecay->SetParName(0, "Norm");
    fDecay->SetParameter(1, 1.203); //iz 20.0 1.203 ± 0.042
    fDecay->SetParName(1, "#tau");

    double sum = 0.0;
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        h->Fill(t);
        sum += t;
    }

    fLN->SetParameter(0, nentries);
    fLN->SetParameter(1, sum);
   
    auto canvas = new TCanvas("canvas", "canvas", 2800, 700);
    gStyle->SetOptFit();
    canvas->Divide(3);

    canvas->cd(1);
    fDecay->SetLineColor(kRed);
    fDecay->Draw();

    h->SetLineColor(kCyan);
    h->SetTitle("Decay; t [s]; N");
    h->Draw("pe1x0");
    h->Fit(fDecay);

    canvas->cd(2);
    fLikelihood->SetLineColor(kGreen);
    fLikelihood->SetTitle("Likelihood Function, t = 1s; #tau [s]; N");
    fLikelihood->Draw();

    canvas->cd(3);
    fLN->SetLineColor(kBlue);
    fLN->SetTitle("Maximum Likelihood; #tau [s]; N");
    fLN->Draw();
    canvas->SaveAs("Decay&Likelihood&LN.pdf");

    std::cout << "Analiticki max Likelihooda = " << sum / nentries << std::endl; //ispadne 1.23506, i bolje je od fita jer fit ovisi o binovima!!!
    std::cout << "Min Likelihood iz 2ln = " << fLN->GetMinimumX() << std::endl;

    double yMin = fLN->GetMinimum(); //ispadne 2422.24

    double x1 = fLN->GetX(yMin + 1.0, 0, fLN->GetMinimumX() - 0.0001);
    double x2 = fLN->GetX(yMin + 1.0, fLN->GetMinimumX() + 0.0001, 1.5);

    //std::cout << "x1 = " << x1 << std::endl; // 0.0124743
    //std::cout << "x2 = " << x2 << std::endl; // 1.27495

    double sigma1 = fLN->GetMinimumX() - x1;
    double sigma2 = x2 - fLN->GetMinimumX();

    std::cout << "Sigma1 za 2ln = " << sigma1 << std::endl;
    std::cout << "Sigma2 za 2ln = " << sigma2 << std::endl;

    double tau_analytical = sum / nentries;
    double sigma_analytical = sqrt(-1.0 * (pow(tau_analytical, 3.0) / nentries * tau_analytical - 2 * sum)); // -1.0 jer nazivnik daje negativno...

    //std::cout << nentries * tau_analytical - 2 * sum << std::endl;
    
    std::cout << "Analiticki sigma = " << sigma_analytical << std::endl; 

}
