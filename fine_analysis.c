//  --- --- --- ---
//
//  Alcor fine tuning macro
//
//  Authors: Chiara Fraticelli, Nicola Rubini
//
//  --- --- --- ---
//
#include "UtilityFunctions.h"
//#include "DataStructure.h"
//
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
        //! Generate fine tune for run1 (18min of 13/11)
        kFineTuneHistograms[kCurrentRun] = uFineTuneAnalysis<TH2F>( kInputFileNames, "output.root" /* , /path/to/ouputfile */);
        cout << "first: " << kFineTuneHistograms[kCurrentRun]->GetName() << endl;
        uBuildNormalizedFineTune( kInputFileNames, kFineTuneHistograms[kCurrentRun], "output2.root" );
    }
}

/*
 void test()
 {
 std::vector<TString> kInputFileNames;
 for ( Int_t iFile = 0; iFile < 24; iFile++ ) {
 kInputFileNames.push_back(Form( /path/to/runspecific/file "./decoded/alcdaq.fifo_%i.root",iFile));
 }
 
 const auto database = uBuildNormalizedFineTune(kInputFileNames);
 
 
 TCanvas* c1 = new TCanvas();
 
 auto h0 = uGetFineTuneHisto<TH2F>(database, uGetIndex(0,0,0,0, false));
 h0->SetLineColor(kRed);
 h0->Draw();
 
 auto h1 = uGetFineTuneHisto<TH2F>(database, uGetIndex(1,0,0,0, false));
 h1->SetLineColor(kYellow);
 h1->Draw("same");
 
 auto h2 = uGetFineTuneHisto<TH2F>(database, uGetIndex(2,0,0,0, false));
 h2->SetLineColor(kGreen);
 h2->Draw("same");
 
 auto h3 = uGetFineTuneHisto<TH2F>(database, uGetIndex(3,0,0,0, false));
 h3->SetLineColor(kBlue);
 h3->Draw("same");
 
 }
 */