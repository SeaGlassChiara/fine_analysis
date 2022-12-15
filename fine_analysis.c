//TODO: cartella graphics
// --- --- --- ---
//
//  Alcor fine tuning macro
//
//  Authors: Chiara Fraticelli, Nicola Rubini
//
//  --- --- --- ---
//
#include "UtilityFunctions.h"
//#include "DataStructure.h"

//for run1
void
fine_analysis
 ( std::vector<Int_t> kRunList ) {
  std::map<TString,TH2F*> kFineTuneHistograms;
    
    for ( auto kCurrentRun : kRunList ) {
        //! Generate file list
        std::vector<TString> kInputFileNames;
        for ( Int_t iFile = 0; iFile < 24; iFile++ ) {
            kInputFileNames.push_back(Form("./decoded%i/alcdaq.fifo_%i.root",kCurrentRun,iFile));
        }
        //! Generate fine tune for run1
        kFineTuneHistograms[kCurrentRun] = uFineTuneAnalysis<TH2F>( kInputFileNames, Form("uTuneALCORout_Run%i.root",kCurrentRun), "./graphics");
        uBuildNormalizedFineTune( kInputFileNames, kFineTuneHistograms[kCurrentRun], Form("TunedALCORout_Run%i.root",kCurrentRun) );
        
     
    }
  
}


  void 
 compareMax()
 {
 
 TFile* kInputFile1 = new TFile ("TunedALCORout_Run1.root");
 const auto database1    = (TH2F*) (kInputFile1->Get("kFine_All_Tune_Params"));
 
 TFile* kInputFile2 = new TFile ("TunedALCORout_Run2.root");
 const auto database2    = (TH2F*) (kInputFile2->Get("kFine_All_Tune_Params"));
 
 //distribution MaxRun1 - MaxRun2
 TH1F*       compareMax    = new TH1F( "compareMax", "MaxRun1-MaxRun2", 100, -1, 1 );
 
 TH1F*       compareMin    = new TH1F( "compareMin", "MinRun1-MinRun2", 100, -1, 1 );
 
  TH1F*       compareSMax    = new TH1F( "compareSMax", "SMaxRun1-SMaxRun2", 100, -1, 1 );
 
 TH1F*       compareSMin    = new TH1F( "compareSMin", "SMinRun1-SMinRun2", 100, -1, 1 );
 
 TH2F*       compareAll    = new TH2F( "compareAll", "All1-All2", 100, -1, 1, 4, 0, 4 );
 
 for(int i=1; i<=kIndexRange; i++){
 
 // float   kCurrentMaxmum1 = database1->GetBinContent( i, 2 );
 // float   kCurrentMaximum2 = database2->GetBinContent( i, 2 );
 
 if (database1->GetBinContent( i, 2 ) == 0) continue;
  if (database2->GetBinContent( i, 2 ) == 0) continue;
 
 if (fabs(database1->GetBinContent( i, 2 )-database2->GetBinContent( i, 2 ))>1) cout << i << " - " << fabs(database1->GetBinContent( i, 2 )-database2->GetBinContent( i, 2 )) << endl;
 compareMax->Fill(database1->GetBinContent( i, 2 )-database2->GetBinContent( i, 2 ));
 
  compareMin->Fill(database1->GetBinContent( i, 4 )-database2->GetBinContent( i, 4 ));
  
   compareSMax->Fill(database1->GetBinContent( i, 3 )-database2->GetBinContent( i, 3 ));
   
    compareSMin->Fill(database1->GetBinContent( i, 5 )-database2->GetBinContent( i, 5 ));
 
 
 compareAll->Fill(database1->GetBinContent( i, 5 )-database2->GetBinContent( i, 5 ), 3.5);
 compareAll->Fill(database1->GetBinContent( i, 4 )-database2->GetBinContent( i, 4 ), 2.5);
 compareAll->Fill(database1->GetBinContent( i, 3 )-database2->GetBinContent( i, 3 ), 1.5);
 compareAll->Fill(database1->GetBinContent( i, 2 )-database2->GetBinContent( i, 2 ), 0.5);
 
 
 }

TCanvas* c1 = new TCanvas();
 compareMax->Draw();
 
 TCanvas* c2 = new TCanvas();
  compareMin->Draw();
  
  TCanvas* c3 = new TCanvas();
   compareSMax->Draw();
   
   TCanvas* c4 = new TCanvas();
    compareSMin->Draw();
    
    TCanvas* c5 = new TCanvas();
     compareAll->Draw();
 
 }

 void 
 draw()
 {
 
 TFile* kInputFile = new TFile ("TunedALCORout_Run1.root");
 
 const auto database    = (TH2F*) (kInputFile->Get("hFine_All_Tuned"));
 
 
 
 TCanvas* c1 = new TCanvas();
 
 auto h0 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,0,0,0, false));
 h0->SetLineColor(kRed);
 h0->Draw();
 
 auto h1 = uGetFineTuneHisto<TH2F>(database, uGetIndex(1,0,0,0, false));
 h1->SetLineColor(kOrange);
 h1->Draw("same");
 
 auto h2 = uGetFineTuneHisto<TH2F>(database, uGetIndex(2,0,0,0, false));
 h2->SetLineColor(kGreen);
 h2->Draw("same");
 
 auto h3 = uGetFineTuneHisto<TH2F>(database, uGetIndex(3,0,0,0, false));
 h3->SetLineColor(kBlue);
 h3->Draw("same");
 
 }
 
 

 
