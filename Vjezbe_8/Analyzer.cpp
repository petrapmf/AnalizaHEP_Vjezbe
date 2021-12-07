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
#include <TF1.h>

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
       TTree* tree = new TTree;

       TFile* f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/public/data/" + path + "/ZZ4lAnalysis.root");
       if (!f || !f->IsOpen())
           f = new TFile("/home/public/data/" + path + "/ZZ4lAnalysis.root");
       TDirectory* dir = (TDirectory*)f->Get("/home/public/data/" + path + "/ZZ4lAnalysis.root:/ZZTree");
       dir->GetObject("candTree", tree);
       Init(tree);
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
               h_signal->Fill(higgs->M(), w);
               //h_signal->Fill(signal_disc, w);
               h2Ds->Fill(higgs->M(), signal_disc, w);
           }
           else if (path.Contains("qqZZ")) {
               bckgrnd_disc = 1.0 / (1.0 + scale * p_QQB_BKG_MCFM / p_GG_SIG_ghg2_1_ghz1_1_JHUGen);
               h_background->Fill(higgs->M(), w);
               //h_background->Fill(bckgrnd_disc, w);
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

       c1->SaveAs("2D.pdf");
       //c1->SaveAs("Dkin.pdf");
   }

   void Analyzer::Fit() {
       auto canvas = new TCanvas("canvas", "canvas", 1600, 800);
       canvas->Divide(2);
       gStyle->SetOptFit();

       canvas->cd(1);
       TF1* fa = new TF1("fa", "[0] + [1]*x + [2]*x*x", 110, 150); //kvadratna Q(x; A, B, C) = A + Bx + Cx^2
       TF1* fb = new TF1("fb", "([0]*[1])/((x*x - [2]*[2])*(x*x - [2]*[2]) + 0.25*[1]*[1])", 110, 150); //Breit-Wigner BW = D*gama/((x^2 - M^2)^2 + 0.25*gama^2)
       TF1* fc = new TF1("fc", "[0] + [1]*x + [2]*x*x + ([3]*[4])/((x*x - [5]*[5])*(x*x - [5]*[5]) + 0.25*[4]*[4])", 110, 150); //suma
       float value_first_parameter = 51.8, value_second_parameter = -0.6213, value_third_parameter = 0.002451; // minjamo 1.0, 0.01 i -0.0001 u parametre dobivene fitanjem
       float value_1st_parameter = 2.042e+04, value_2nd_parameter = 829.9, value_3rd_parameter = 124.4; // minjamo 70.0, 200.0 i 125.0 u parametre dobivene fitanjem
      
       fa->SetParameter(0, value_first_parameter);
       fa->SetParameter(1, value_second_parameter);
       fa->SetParameter(2, value_third_parameter);

       fb->SetParameter(0, value_1st_parameter);
       fb->SetParameter(1, value_2nd_parameter);
       fb->SetParameter(2, value_3rd_parameter);

       fc->SetParameter(0, value_first_parameter);
       fc->SetParameter(1, value_second_parameter);
       fc->SetParameter(2, value_third_parameter);
       fc->SetParameter(3, value_1st_parameter);
       fc->SetParameter(4, value_2nd_parameter);
       fc->SetParameter(5, value_3rd_parameter);

       fa->SetParName(0, "A");
       fa->SetParName(1, "B");
       fa->SetParName(2, "C");

       fb->SetParName(0, "D");
       fb->SetParName(1, "#Gamma");
       fb->SetParName(2, "M");

       fc->SetParName(0, "A");
       fc->SetParName(1, "B");
       fc->SetParName(2, "C");
       fc->SetParName(3, "D");
       fc->SetParName(4, "#Gamma");
       fc->SetParName(5, "M");

       fa->SetLineColor(kYellow);
       fb->SetLineColor(kBlue);
       fc->SetLineColor(kGreen);

       fc->SetTitle("Model; M [GeV]; # of Events");

       fc->Draw();
       fb->Draw("same");
       fa->Draw("same");

       TLegend* legend = new TLegend(.7, .75, .89, .89);
       legend->AddEntry(fa, "Background", "l");
       legend->AddEntry(fb, "Signal", "l");
       legend->AddEntry(fc, "Background + Signal", "l");
       legend->Draw();

       canvas->cd(2);
       h_signal->SetLineColor(kBlue);
       h_background->SetLineColor(kYellow);
       h_signal->Add(h_background);
       h_signal->GetXaxis()->SetRangeUser(110.0, 150.0);
       h_signal->GetYaxis()->SetRangeUser(0.0, 120.0);

       h_signal->Draw("pe1x0");
       h_signal->SetTitle("Reconstructed Mass; M [GeV]; # of Events");
       h_signal->Fit(fc);

       canvas->SaveAs("Q&BW&Fit.pdf");
   }

   void Analyzer::FitML() {
       auto c = new TCanvas("canvas", "canvas", 1600, 800);

       TF1* fML = new TF1("fML", "[0] + [1]*x + [2]*x*x + ([3]*[4])/((x*x - [5]*[5])*(x*x - [5]*[5]) + 0.25*[4]*[4]) + ([6]*[7])/((x*x - [8]*[8])*(x*x - [8]*[8]) + 0.25*[7]*[7])", 70.0, 170.0);
       gStyle->SetOptFit();

       float value_first_parameter = -66.75, value_second_parameter = 1.244, value_third_parameter = -0.00459; // minjamo 1.0, 0.01 i -0.0001 u parametre dobivene fitanjem
       float value_1st_parameter = 5412.0, value_2nd_parameter = 83.84, value_3rd_parameter = 125.3; // minjamo 70.0, 200.0 i 125.0 u parametre dobivene fitanjem
       float value_1_parameter = 3.251e+04, value_2_parameter = 837.6, value_3_parameter = 90.7;

       fML->SetParameter(0, value_first_parameter);
       fML->SetParameter(1, value_second_parameter);
       fML->SetParameter(2, value_third_parameter);

       fML->SetParameter(3, value_1st_parameter);
       fML->SetParameter(4, value_2nd_parameter);
       fML->SetParameter(5, value_3rd_parameter);

       fML->SetParameter(6, value_1_parameter);
       fML->SetParameter(7, value_2_parameter);
       fML->SetParameter(8, value_3_parameter);

       fML->SetParName(0, "A");
       fML->SetParName(1, "B");
       fML->SetParName(2, "C");
       fML->SetParName(3, "D_{s}");
       fML->SetParName(4, "#Gamma_{s}");
       fML->SetParName(5, "M_{s}");
       fML->SetParName(6, "D_{b}");
       fML->SetParName(7, "#Gamma_{b}");
       fML->SetParName(8, "M_{b}");

       fML->SetLineColor(kCyan);
       fML->Draw();

       h_signal->SetLineColor(kBlue);
       h_signal->Add(h_background);
       h_signal->SetTitle("Reconstructed Mass; M [GeV]; # of Events");
       h_signal->Draw("pe1x0");
       h_signal->Fit(fML);

       c->SaveAs("MLfit.pdf");
   }