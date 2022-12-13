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
 ( std::vector<TString> kRunList ) {
    std::map<TString,TH2F*> kFineTuneHistograms;
    for ( auto kCurrentRun : kRunList ) {
        //! Generate file list
        std::vector<TString> kInputFileNames;
        for ( Int_t iFile = 0; iFile < 24; iFile++ ) {
            kInputFileNames.push_back(Form(/* /path/to/runspecific/file */"./decoded/alcdaq.fifo_%i.root",iFile));
        }
        //! Generate fine tune for run1
        kFineTuneHistograms[kCurrentRun] = uFineTuneAnalysis<TH2F>( kInputFileNames, "output.root", "./graphics");
        uBuildNormalizedFineTune( kInputFileNames, kFineTuneHistograms[kCurrentRun], "output2.root" );
        
    }
}

 void 
 draw()
 {
 std::vector<TString> kInputFileNames;
 for ( Int_t iFile = 0; iFile < 24; iFile++ ) {
 kInputFileNames.push_back(Form( /* /path/to/runspecific/file */ "./decoded/alcdaq.fifo_%i.root",iFile));
 }
 
 const auto tuning      = uFineTuneAnalysis<TH2F>(kInputFileNames, "uTuneALCORout.root");
 const auto database    = uBuildNormalizedFineTune(kInputFileNames, tuning, "TunedALCORout.root" );
 
 
 TCanvas* c1 = new TCanvas();
 
 auto h0 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,1,1,0, false));
 h0->SetLineColor(kRed);
 h0->Draw();
 
 auto h1 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,1,1,1, false));
 h1->SetLineColor(kOrange);
 h1->Draw("same");
 
 auto h2 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,1,1,2, false));
 h2->SetLineColor(kGreen);
 h2->Draw("same");
 
 auto h3 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,1,1,3, false));
 h3->SetLineColor(kBlue);
 h3->Draw("same");
 
 }
 
 
 
