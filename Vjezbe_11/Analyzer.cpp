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

double Analyzer::Factorial(int n) {
    double f = 1.0;

    if (n == 0.0) {
        f = 1.0;
    }
    else {
        for (int i = 1; i <= n; i++) {
            f *= i;
        }
    }
    return f;
}
double Analyzer::Binomial(int r, double p, int N) {
    double bin = (Factorial(N) / (Factorial(r) * Factorial(N - r))) * pow(p, r) * pow(1.0 - p, N - r);
    //std::cout << "Binomial = " << bin << std::endl;
    return bin;
}

double Analyzer::UpperCPInterval(int m, int N1, double C) {
    double p_upper = 1.0;
    double sum = 0.0, sum_temp = 0.0;
    int r1;


    while (sum < (1.0 - C) / 2.0) {
        for (r1 = m + 1; r1 <= N1; r1++) {
            sum_temp += Binomial(r1, p_upper, N1);
        }
        sum = 1.0 - sum_temp;
        p_upper -= 0.001;
        sum_temp = 0.0;
    }
    return p_upper;
}

double Analyzer::LowerCPInterval(int m, int N1, double C) {
    double p_lower = 0.0;
    double sum = 0.0, sum_temp = 0.0;
    int r1;


    while (sum < (1.0 - C) / 2.0) {
        for (r1 = 0; r1 <= m - 1; r1++) {
            sum_temp += Binomial(r1, p_lower, N1);
        }
        sum = 1.0 - sum_temp;
        p_lower += 0.001;
        sum_temp = 0.0;
    }
    return p_lower;
}

void Analyzer::ClopperPearson(int N, double C) {
    double p_upper, p_lower;
    int r;

    for (r = 0; r <= N; r++) {
        p_upper = UpperCPInterval(r, N, C);
        p_lower = LowerCPInterval(r, N, C);
        std::cout << r << " successes out of 10 events: " << "[" << p_lower << "," << p_upper << "]" << std::endl;
    }
}

void Analyzer::PlotCPBelt(int N, double C) {
    double p_upper, p_lower;

    TH1F* h_upper = new TH1F("h_upper", "Clopper-Pearson Confidence Belt; p; r", N, 0, N);
    TH1F* h_lower = new TH1F("h_lower", "Clopper-Pearson Confidence Belt; p; r", N, 0, N);

    for (int r = 0; r <= N; r++) {
        p_upper = UpperCPInterval(r, N, C);
        p_lower = LowerCPInterval(r, N, C);

        h_upper->SetBinContent(r, p_upper);
        h_lower->SetBinContent(r, p_lower);
    }

    auto canvas = new TCanvas("canvas", "Clopper-Pearson", 1600, 900);
    gStyle->SetOptStat(0);

    h_upper->SetLineColor(kViolet);
    h_upper->SetFillColor(kViolet);
    //h_lower->SetFillColor(kWhite); ne funkcionira ka i ostale boje, triba definirat neku "drugu" bijelu
    Int_t ci = TColor::GetFreeColorIndex();
    TColor* white = new TColor(ci, 1., 1., 1.);
    h_lower->SetFillColor(ci);
    h_lower->SetLineColor(kViolet);

    h_upper->Draw();
    h_lower->Draw("same");

    canvas->SaveAs("Clopper-Pearson Belt.pdf");
}

void Analyzer::Dice(int N, double C) {
    srand(time(NULL));
    int br, ptrue = 0;
    double p_upper, p_lower;

    for (int i = 0; i < 1000; i++) {
        br = 0;
        for (int j = 0; j < N; j++) {
            if (rand() % 6 == 0) {
                br++;
            }
        }

        p_upper = UpperCPInterval(br, N, C);
        p_lower = LowerCPInterval(br, N, C);

        if (p_upper >= 1. / 6 && p_lower <= 1. / 6) { //true value je 1/6
            ptrue++;
        }
    }

    std::cout << "# of experiments that contain true value: " << ptrue << std::endl;
}

