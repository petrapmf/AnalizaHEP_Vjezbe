#define Analyzer_cxx
#include "Analyzer.h"
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
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
    }
}

   void Analyzer::Plot() 
   {

       TH1F* hpt1 = new TH1F("hpt1", "Lepton Transversal Momentum", 100, 0, 100);
       TH1F* hpt2 = new TH1F("hpt2", "Lepton Transversal Momentum", 100, 0, 100);
       TH1F* hpt3 = new TH1F("hpt3", "Lepton Transversal Momentum", 100, 0, 100);
       TH1F* hpt4 = new TH1F("hpt4", "Lepton Transversal Momentum", 100, 0, 100);

       TH1F* heta1 = new TH1F("heta1", "Lepton Pseudorapidity", 100, -5, 5);
       TH1F* heta2 = new TH1F("heta2", "Lepton Pseudorapidity", 100, -5, 5);
       TH1F* heta3 = new TH1F("heta3", "Lepton Pseudorapidity", 100, -5, 5);
       TH1F* heta4 = new TH1F("heta4", "Lepton Pseudorapidity", 100, -5, 5);

       TH1F* hphi1 = new TH1F("hphi1", "Azimuthal angle", 100, -5, 5);
       TH1F* hphi2 = new TH1F("hphi2", "Azimuthal angle", 100, -5, 5);
       TH1F* hphi3 = new TH1F("hphi3", "Azimuthal angle", 100, -5, 5);
       TH1F* hphi4 = new TH1F("hphi4", "Azimuthal angle", 100, -5, 5);

       TH1F* hbdt1 = new TH1F("hbdt1", "Boosted Decision Tree", 40, 0, 10);
       TH1F* hbdt2 = new TH1F("hbdt2", "Boosted Decision Tree", 40, 0, 10);
       TH1F* hbdt3 = new TH1F("hbdt3", "Boosted Decision Tree", 40, 0, 10);
       TH1F* hbdt4 = new TH1F("hbdt4", "Boosted Decision Tree", 40, 0, 10);

       /*TLorentzVector* higgs = new TLorentzVector();
       TLorentzVector* Z_e = new TLorentzVector();
       TLorentzVector* Z_mi = new TLorentzVector();
       TLorentzVector* lep_e1 = new TLorentzVector();
       TLorentzVector* lep_mi1 = new TLorentzVector();
       TLorentzVector* lep_e2 = new TLorentzVector();
       TLorentzVector* lep_mi2 = new TLorentzVector();

       lep_e1->SetPxPyPz()
       *Z_e = *lep_e1 + *lep_e2;
       *Z_mi = *lep_mi1 + *lep_mi2; */

       if (fChain == 0) return;

       Long64_t nentries = fChain->GetEntriesFast();

       Long64_t nbytes = 0, nb = 0;
       for (Long64_t jentry = 0; jentry < nentries; jentry++) {
           Long64_t ientry = LoadTree(jentry);
           if (ientry < 0) break;
           nb = fChain->GetEntry(jentry);   nbytes += nb;

           hpt1->Fill(LepPt->at(2));
           hpt2->Fill(LepPt->at(3));
           hpt3->Fill(LepPt->at(0));
           hpt4->Fill(LepPt->at(1));

           heta1->Fill(LepEta->at(2));
           heta2->Fill(LepEta->at(3));
           heta3->Fill(LepEta->at(0));
           heta4->Fill(LepEta->at(1));

           hphi1->Fill(LepPhi->at(2));
           hphi2->Fill(LepPhi->at(3));
           hphi3->Fill(LepPhi->at(0));
           hphi4->Fill(LepPhi->at(1));

           hbdt1->Fill(LepBDT->at(2));
           hbdt2->Fill(LepBDT->at(3));
           hbdt3->Fill(LepBDT->at(0));
           hbdt4->Fill(LepBDT->at(1));
           
       }
       auto canvas = new TCanvas("canvas", "canvas", 1200, 800);
       canvas->Divide(2, 2);

       gStyle->SetOptStat(0);
       canvas->cd(1);
       hpt1->GetXaxis()->SetTitle("p_{T} [GeV]");
       hpt1->GetYaxis()->SetTitle("# of Events");
       hpt1->SetLineColor(kRed);
       hpt1->Draw();

       hpt2->GetXaxis()->SetTitle("p_{T} [GeV]");
       hpt2->GetYaxis()->SetTitle("# of Events");
       hpt2->SetLineColor(kBlue);
       hpt2->Draw("hist same");

       hpt3->GetXaxis()->SetTitle("p_{T} [GeV]");
       hpt3->GetYaxis()->SetTitle("# of Events");
       hpt3->SetLineColor(kGreen);
       hpt3->Draw("hist same");

       hpt4->GetXaxis()->SetTitle("p_{T} [GeV]");
       hpt4->GetYaxis()->SetTitle("# of Events");
       hpt4->SetLineColor(kMagenta);
       hpt4->Draw("hist same");

       TLegend* legend1 = new TLegend(.7, .75, .89, .89);
       legend1->AddEntry(hpt1, "Lepton 1", "l");
       legend1->AddEntry(hpt2, "Lepton 2", "l");
       legend1->AddEntry(hpt3, "Lepton 3", "l");
       legend1->AddEntry(hpt4, "Lepton 4", "l");
       legend1->Draw();

       canvas->cd(2);
       heta1->GetXaxis()->SetTitle("#eta");
       heta1->GetYaxis()->SetTitle("# of Events");
       heta1->SetLineColor(kRed);
       heta1->Draw();

       heta2->GetXaxis()->SetTitle("#eta");
       heta2->GetYaxis()->SetTitle("# of Events");
       heta2->SetLineColor(kBlue);
       heta2->Draw("hist same");

       heta3->GetXaxis()->SetTitle("#eta");
       heta3->GetYaxis()->SetTitle("# of Events");
       heta3->SetLineColor(kGreen);
       heta3->Draw("hist same");

       heta4->GetXaxis()->SetTitle("#eta");
       heta4->GetYaxis()->SetTitle("# of Events");
       heta4->SetLineColor(kMagenta);
       heta4->Draw("hist same");

       TLegend* legend2 = new TLegend(.7, .75, .89, .89);
       legend2->AddEntry(heta1, "Lepton 1", "l");
       legend2->AddEntry(heta2, "Lepton 2", "l");
       legend2->AddEntry(heta3, "Lepton 3", "l");
       legend2->AddEntry(heta4, "Lepton 4", "l");
       legend2->Draw();

       canvas->cd(3);
       hphi1->GetXaxis()->SetTitle("#phi [rad]");
       hphi1->GetYaxis()->SetTitle("# of Events");
       hphi1->SetLineColor(kRed);
       hphi1->Draw();

       hphi2->GetXaxis()->SetTitle("#phi [rad]");
       hphi2->GetYaxis()->SetTitle("# of Events");
       hphi2->SetLineColor(kBlue);
       hphi2->Draw("hist same");

       hphi3->GetXaxis()->SetTitle("#phi [rad]");
       hphi3->GetYaxis()->SetTitle("# of Events");
       hphi3->SetLineColor(kGreen);
       hphi3->Draw("hist same");

       hphi4->GetXaxis()->SetTitle("#phi [rad]");
       hphi4->GetYaxis()->SetTitle("# of Events");
       hphi4->SetLineColor(kMagenta);
       hphi4->Draw("hist same");

       canvas->cd(4);
       hbdt1->GetXaxis()->SetTitle("BDT");
       hbdt1->GetYaxis()->SetTitle("# of Events");
       hbdt1->SetLineColor(kRed);
       hbdt1->Draw();

       hbdt2->GetXaxis()->SetTitle("BDT");
       hbdt2->GetYaxis()->SetTitle("# of Events");
       hbdt2->SetLineColor(kBlue);
       hbdt2->Draw("hist same");

       hbdt3->GetXaxis()->SetTitle("BDT");
       hbdt3->GetYaxis()->SetTitle("# of Events");
       hbdt3->SetLineColor(kGreen);
       hbdt3->Draw("hist same");

       hbdt4->GetXaxis()->SetTitle("BDT");
       hbdt4->GetYaxis()->SetTitle("# of Events");
       hbdt4->SetLineColor(kMagenta);
       hbdt4->Draw("hist same");

       TLegend* legend4 = new TLegend(.7, .75, .89, .89);
       legend4->AddEntry(heta1, "Lepton 1", "l");
       legend4->AddEntry(heta2, "Lepton 2", "l");
       legend4->AddEntry(heta3, "Lepton 3", "l");
       legend4->AddEntry(heta4, "Lepton 4", "l");
       legend4->Draw();

       canvas->SaveAs("Lepton.png");
       canvas->SaveAs("Lepton.pdf");
       canvas->SaveAs("Lepton.root");
}

