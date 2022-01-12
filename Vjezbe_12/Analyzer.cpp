#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

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

    Init(signal);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      hs_pt->Fill(ele_pt);
   }

   Init(bckg);

   if (fChain == 0) return;

   nentries = fChain->GetEntriesFast();

   nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++) {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   nbytes += nb;
       hb_pt->Fill(ele_pt);
   }
}

void Analyzer::Plot() {
    auto c = new TCanvas("c", "c", 1300, 1300);
    c->Divide(4, 4);
    gStyle->SetOptStat(0);

    c->cd(1);
    hs_pt->GetXaxis()->SetTitle("p_{T} [GeV]");
    hs_pt->GetYaxis()->SetTitle("# of Events");
    hs_pt->SetLineColor(kRed);
    hs_pt->Draw("hist");

    hb_pt->GetXaxis()->SetTitle("p_{T} [GeV]");
    hb_pt->GetYaxis()->SetTitle("# of Events");
    hb_pt->SetLineColor(kBlue);
    hb_pt->Draw("hist same");

    c->SaveAs("Plot.pdf");
}

void Analyzer::TMVAClassification()
{
    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
    TString outfileName("TMVA.root");
    TFile* outputFile = TFile::Open(outfileName, "RECREATE");
    // Create the factory object. Later you can choose the methods
    // whose performance you'd like to investigate. The factory is
    // the only TMVA object you have to interact with
    //
    // The first argument is the base of the name of all the
    // weightfiles in the directory weight/
    //
    // The second argument is the output file for the training results
    // All TMVA output can be suppressed by removing the "!" (not) in
    // front of the "Silent" argument in the option string
    TMVA::Factory* factory = new TMVA::Factory("TMVAClassification", outputFile,
        "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
    TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset");
    // If you wish to modify default settings
    // (please check "src/Config.h" to see all available global options)
    //
    //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
    //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
    // Define the input variables that shall be used for the MVA training
    // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
    // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
    dataloader->AddVariable("ele_pt", 'F');
    dataloader->AddVariable("scl_eta", 'F');
    dataloader->AddVariable("ele_hadronicOverEm", 'F');
   
    // You can add so-called "Spectator variables", which are not used in the MVA training,
    // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
    // input variables, the response values of all trained MVAs, and the spectator variables
    //dataloader->AddSpectator("spec1 := var1*2", "Spectator 1", "units", 'F');
    //dataloader->AddSpectator("spec2 := var1*3", "Spectator 2", "units", 'F');
    // global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;
    // You can add an arbitrary number of signal or background trees
    dataloader->AddSignalTree(signal, signalWeight);
    dataloader->AddBackgroundTree(bckg, backgroundWeight);
    // To give different trees for training and testing, do as follows:
   // dataloader->SetBackgroundWeightExpression("weight");
    // Apply additional cuts on the signal and background samples (can be different)
    TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
    TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";
    // Tell the dataloader how to use the training and testing events
    //
    // If no numbers of events are given, half of the events in the tree are used
    // for training, and the other half for testing:
    //
    //    dataloader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
    //
    // To also specify the number of testing events, use:
    //
    //    dataloader->PrepareTrainingAndTestTree( mycut,
    //         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
    dataloader->PrepareTrainingAndTestTree(mycuts, mycutb,
        "nTrain_Signal=1000:nTrain_Background=1000:SplitMode=Random:NormMode=NumEvents:!V");
    // ### Book MVA methods
    //
    // Please lookup the various method configuration options in the corresponding cxx files, eg:
    // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
    // it is possible to preset ranges in the option string in which the cut optimisation should be done:
    // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable
    // Cut optimisation
        factory->BookMethod(dataloader, TMVA::Types::kLikelihood, "Likelihood",
            "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50");
    
    factory->TrainAllMethods();
    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();
    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    // --------------------------------------------------------------
    // Save the output
    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete factory;
    delete dataloader;
    // Launch the GUI for the root mac
}