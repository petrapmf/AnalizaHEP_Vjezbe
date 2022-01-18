#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TColor.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TLorentzVector.h>
#include <THStack.h>

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
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
    }
}

   void Analyzer::Plot(TString path) 
   {
       THStack* hs = new THStack("hs", "hs"); //THStack --> collection of TH1 (or derived) objects
       TFile* f = new TFile("/home/public/data/" + path + "/ZZ4lAnalysis.root");
       TTree* tree = (TTree*) f->Get("ZZTree/candTree");
       Init(tree); //nadodano iz .h, al bez onih ifova
       TH1F* histoCounter = (TH1F*)f->Get("ZZTree/Counters");
       float binContent = histoCounter->GetBinContent(40);

       TLorentzVector* higgs = new TLorentzVector();
       TLorentzVector* Z1 = new TLorentzVector();
       TLorentzVector* Z2 = new TLorentzVector();
       TLorentzVector* lep0 = new TLorentzVector();
       TLorentzVector* lep1 = new TLorentzVector();
       TLorentzVector* lep2 = new TLorentzVector();
       TLorentzVector* lep3 = new TLorentzVector();

       float signal_disc;
       float bckgrnd_disc;
       float scale = 70.0;
       //TH1F* histohiggs = new TH1F("histohiggs", "Higgs Mass", 25, 90, 140); //no. of bins = ? -> size = 50; one bin = 2; 50 / 2 = 25 

       //to read a histogram from another Root session:
       //TFile f("/home/public/data/ggH125/ZZ4lAnalysis.root"); 
       //TH1F* histoCounter = (TH1F*)f->Get("ZZTree/Counters");

       if (fChain == 0) return;

       Long64_t nentries = fChain->GetEntriesFast();

       Long64_t nbytes = 0, nb = 0;
       for (Long64_t jentry = 0; jentry < nentries; jentry++) {
           Long64_t ientry = LoadTree(jentry);
           if (ientry < 0) break;
           nb = fChain->GetEntry(jentry);   nbytes += nb;

           //float binContent = histoCounter->GetBinContent(40);
           //std::cout << binContent << std::endl;
           float w = (137.0 * 1000.0 * xsec * overallEventWeight) / binContent;

           lep0->SetPtEtaPhiM(LepPt->at(0), LepEta->at(0), LepPhi->at(0), 0);
           lep1->SetPtEtaPhiM(LepPt->at(1), LepEta->at(1), LepPhi->at(1), 0);
           lep2->SetPtEtaPhiM(LepPt->at(2), LepEta->at(2), LepPhi->at(2), 0);
           lep3->SetPtEtaPhiM(LepPt->at(3), LepEta->at(3), LepPhi->at(3), 0);

           *Z1 = *lep0 + *lep1;
           *Z2 = *lep2 + *lep3;

           *higgs = *Z1 + *Z2;
           
           if (path.Contains("ggH125")) {
               signal_disc = 1.0 / (1.0 + p_QQB_BKG_MCFM / p_GG_SIG_ghg2_1_ghz1_1_JHUGen);
               //h_signal->Fill(higgs->M(), w);
               h_signal->Fill(signal_disc, w);
               h2Ds->Fill(higgs->M(), signal_disc, w);
           }
           else if (path.Contains("qqZZ")) {
               bckgrnd_disc = 1.0 / (1.0 + scale * p_QQB_BKG_MCFM / p_GG_SIG_ghg2_1_ghz1_1_JHUGen);
               //h_background->Fill(higgs->M(), w);
               h_background->Fill(bckgrnd_disc, w);
               h2Db->Fill(higgs->M(), bckgrnd_disc, w);
           }
           //histohiggs->Fill(higgs->M(), w);
       }
       
   }

   void Analyzer::DrawDkin() {
       auto c1 = new TCanvas("c1", "c1", 1300, 800);
       c1->Divide(2, 2);
       gStyle->SetOptStat(0);

       c1->cd(1);
       h_signal->Scale(1.0 / h_signal->Integral());
       h_signal->GetXaxis()->SetTitle("D_{kin}");
       h_signal->GetYaxis()->SetTitle("# of Events");
       h_signal->SetLineColor(kRed);
       h_signal->Draw("hist");

       h_background->Scale(1.0 / h_background->Integral());
       h_background->GetXaxis()->SetTitle("D_{kin}");
       h_background->GetYaxis()->SetTitle("# of Events");
       h_background->SetLineColor(kCyan);
       h_background->Draw("hist same");

       TLegend* legend = new TLegend(.7, .75, .89, .89);
       legend->AddEntry(h_signal, "ggH125", "l");
       legend->AddEntry(h_background, "qqZZ", "l");
       legend->Draw();

       c1->cd(2);
       auto roc = new TGraph();

       for (int i = 0; i < 100000; i++) { //vidi Dkin graf!! 

           float sig_integral = h_signal->Integral(1, i + 1); //od prvog bina nadalje
           float bkg_integral = h_background->Integral(1, i + 1);
           float x = 1.0 - sig_integral;
           float y = 1.0 - bkg_integral;
           if (x > 0.01 && y > 0.01 && x < 1.0 && y < 1.0)
               roc->SetPoint(i, x, y);
       }

       roc->GetXaxis()->SetTitle("Background");
       roc->GetYaxis()->SetTitle("Signal");
       roc->SetTitle("ROC curve");
       roc->Draw("AP");

       c1->cd(3);
       h2Db->SetTitle("Background");
       h2Db->GetXaxis()->SetTitle("m_{4l} [GeV]");
       h2Db->GetYaxis()->SetTitle("D_{kin}");
       h2Db->SetMinimum(-0.01);
       h2Db->Draw("COLZ");

       c1->cd(4);
       h2Ds->SetTitle("Signal");
       h2Ds->GetXaxis()->SetTitle("m_{4l} [GeV]");
       h2Ds->GetYaxis()->SetTitle("D_{kin}");
       h2Ds->SetMinimum(-0.01);
       h2Ds->Draw("COLZ");

       c1->SaveAs("ROC.pdf");
       //c1->SaveAs("2D.pdf");
       //c1->SaveAs("Dkin.pdf");
   }
