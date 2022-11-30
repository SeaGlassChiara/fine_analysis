#ifndef ALCOR_UTILITYFUNCTIONS_H
#define ALCOR_UTILITYFUNCTIONS_H
//
//  --- --- --- ---
//
//  Alcor readout utilities
//
//  Authors: Chiara Fraticelli, Nicola Rubini
//
//  --- --- --- ---
//
#include "DataStructure.h"
//
//  Utility TF1 Functions
auto uDoubleFermi = new TF1("DoubleFermi", "[0]*(1./(exp((x-[1])/[2])+1))*(1./(exp(([3]-x)/[4])+1))", 20., 150.);
void
uSetDoubleFermi
( Float_t nMaximum ) {
    // Set Parameter 0
    uDoubleFermi->SetParLimits(0, 0, 1e4);
    uDoubleFermi->SetParameter(0, nMaximum );
    // Set Parameter 1
    uDoubleFermi->SetParLimits(1, 70, 200);
    uDoubleFermi->SetParameter(1, 100);
    // Set Parameter 2
    uDoubleFermi->SetParLimits(2, 0, 1.);
    uDoubleFermi->SetParameter(2, 0.4);
    // Set Parameter 3
    uDoubleFermi->SetParLimits(3, 0, 70);
    uDoubleFermi->SetParameter(3, 40.);
    // Set Parameter 4
    uDoubleFermi->SetParLimits(4, 0, 1.);
    uDoubleFermi->SetParameter(4, 0.4);
}
//  --- --- ---
template< typename TH2_Type = TH2F >
TH2_Type*
uBuildFineTune
 ( std::vector<TString> kInputFileNames ) {
    //! histogram with fine distribution
    TH2_Type* hFine_All = new TH2_Type("hFine_All", ";index;fine", 768, 0, 768, 512, 0, 512); //! TODO: change to centralized index
    //
    //! Loop on filenames
    for ( auto kCurrentFileName : kInputFileNames ) {
        std::cout << "[INFO] Opening file: " << kCurrentFileName.Data() << std::endl;
        //! Load File
        auto kCurrentFile = TFile::Open(kCurrentFileName);
        if ( !kCurrentFile || !kCurrentFile->IsOpen() ) {
            std::cout << "[WARNING] Opening file: " << kCurrentFileName.Data() << " failed!" << std::endl;
            continue;
        }
        //! Load Tree
        ALCORout    kCurrentData;
        TTree*      kCurrentTree = (TTree *)kCurrentFile->Get("alcor");
        if ( !kCurrentTree ) {
            std::cout << "[WARNING] No \"alcor\" tree in file: " << kCurrentFileName.Data() << ". Loading failed!" << std::endl;
            continue;
        }
        //! Get Entries and loop to fill histogram
        auto nEvents = kCurrentTree->GetEntries();
        LoadTree( kCurrentTree, kCurrentData );
        for (int iEv = 0; iEv < nEvents; ++iEv) {
            kCurrentTree->GetEntry(iEv);
            int iCurrentIndex = uGetIndex( kCurrentData.fifo, kCurrentData.pixel, kCurrentData.column, kCurrentData.tdc );
            hFine_All->Fill( iCurrentIndex, kCurrentData.fine);
        }
        kCurrentFile->Close();
  }
  return hFine_All;
}
//  --- --- ---
template< typename TH2_Type = TH2F >
TH1D*
uGetFineTuneHisto
 ( TH2_Type* hFine_All, Int_t iIndex ) {
    return hFine_All->ProjectionY( Form("hFine_%i", iIndex ), iIndex + 1, iIndex + 1);
}
//  --- --- ---
template< typename TH2_Type = TH2F >
TH2F*
uFineTuneAnalysis
( std::vector<TString> kInputFileNames, TString kOutputFileName = "", TString kOutputGraphics = "" ) {
    //! Run in Batch mode
    gROOT->SetBatch(true);
    //! Load Files
    auto hFine_All = uBuildFineTune<TH2F>( kInputFileNames );
    //! Create output
    TH2_Type*   kFine_All_Tune_Params   = new TH2_Type( "kFine_All_Tune_Params", "kFine_All_Tune_Params", kIndexRange, -0.5, kIndexRange-0.5, 5, 0., 5.);
    TH1F*       kMaximumDistribution    = new TH1F( "kMaximumDistribution", "kMaximumDistribution", 200, 0, 200 );
    TH1F*       kMinimumDistribution    = new TH1F( "kMinimumDistribution", "kMinimumDistribution", 200, 0, 200 );
    kFine_All_Tune_Params->GetYaxis()->SetBinLabel(1,"Normalisation");
    kFine_All_Tune_Params->GetYaxis()->SetBinLabel(2,"Maximum");
    kFine_All_Tune_Params->GetYaxis()->SetBinLabel(3,"Sigma Max");
    kFine_All_Tune_Params->GetYaxis()->SetBinLabel(4,"Minimum");
    kFine_All_Tune_Params->GetYaxis()->SetBinLabel(5,"Sigma Min");
    //! Loop on found infos
    for ( Int_t iIndex = 0; iIndex < kIndexRange; iIndex++ ) {
        auto kCurrentInfos      = uGetInfos(iIndex);
        auto kCurrentFineHisto  = uGetFineTuneHisto( hFine_All, iIndex );
        if ( kCurrentFineHisto->GetEntries() <= 100 ) {
            cout << "[WARNING] Skipping empty histogram " << iIndex << " chip:" << get<0>(kCurrentInfos) << " pixel:" <<  get<1>(kCurrentInfos) << " colum:" <<  get<2>(kCurrentInfos) << " TDC:" << get<3>(kCurrentInfos)  << endl;
            continue;
        }
        uSetDoubleFermi( kCurrentFineHisto->GetBinContent(70) );
        kCurrentFineHisto->Fit( uDoubleFermi, "SQ", "" );
        kFine_All_Tune_Params->SetBinContent( iIndex+1, 1, uDoubleFermi->GetParameter(0) );
        kFine_All_Tune_Params->SetBinContent( iIndex+1, 2, uDoubleFermi->GetParameter(1) );
        kFine_All_Tune_Params->SetBinContent( iIndex+1, 3, uDoubleFermi->GetParameter(2) );
        kFine_All_Tune_Params->SetBinContent( iIndex+1, 4, uDoubleFermi->GetParameter(3) );
        kFine_All_Tune_Params->SetBinContent( iIndex+1, 5, uDoubleFermi->GetParameter(4) );
        kMaximumDistribution->Fill(uDoubleFermi->GetParameter(1));
        kMinimumDistribution->Fill(uDoubleFermi->GetParameter(3));
        if ( !(kOutputGraphics.Length() == 0) ) {
            TLatex* kLatex = new TLatex();
            auto kMinimum = uDoubleFermi->GetParameter(3);
            auto kMaximum = uDoubleFermi->GetParameter(1);
            TCanvas* c1 = new TCanvas("", "", 1000, 300);
            c1->Divide(3,1);
            c1->cd(1);
            kCurrentFineHisto->GetXaxis()->SetRangeUser(kMinimum -10, kMinimum +20);
            kCurrentFineHisto->DrawCopy();
            kLatex->DrawLatexNDC(0.6, 0.5, Form("Min: %.2f", kMinimum));
            c1->cd(2);
            kCurrentFineHisto->GetXaxis()->SetRangeUser(kMinimum -10, kMaximum +10);
            kCurrentFineHisto->DrawCopy();
            kLatex->DrawLatexNDC(0.4, 0.5, Form("Max: %.2f", kMaximum));
            kLatex->DrawLatexNDC(0.4, 0.45, Form("Min: %.2f", kMinimum));
            c1->cd(3);
            kCurrentFineHisto->GetXaxis()->SetRangeUser(kMaximum -20, kMaximum +10);
            kCurrentFineHisto->DrawCopy();
            kLatex->DrawLatexNDC(0.3, 0.5, Form("Max: %.2f", kMaximum));
            
            
            //uDoubleFermi->Draw("SAME");
            c1->SaveAs(Form("%s/%s.pdf",kOutputGraphics.Data(),kCurrentFineHisto->GetName()));
        }
    }
    //! Save output
    if ( kOutputFileName.Length() != 0 ) {
        TFile*  kFileOut = new TFile( kOutputFileName, "RECREATE" );
        kMaximumDistribution->Write();
        kMinimumDistribution->Write();
        hFine_All->Write();
        kFine_All_Tune_Params->Write();
        kFileOut->Close();
    }
    //! Run in Batch mode
    gROOT->SetBatch(false);
    return kFine_All_Tune_Params;
}
//  --- --- ---
//
template< typename TH2_Type = TH2F >
TH2_Type*
uBuildNormalizedFineTune
 ( std::vector<TString> kInputFileNames, TH2F* kFine_All_Tune_Params, TString kOutputFileName = "", TString kOutputGraphics = ""  ) {
    //! Create output
    TH2_Type* hFine_All_Tuned = new TH2_Type("hFine_All_Tuned", "hFine_All_Tuned", kIndexRange, -0.5, kIndexRange-0.5, 120, -20, 100 );
    //! Loop on filenames
    for ( auto kCurrentFileName : kInputFileNames ) {
        std::cout << "[INFO] Opening file: " << kCurrentFileName.Data() << std::endl;
        //! Load File
        auto kCurrentFile = TFile::Open(kCurrentFileName);
        if ( !kCurrentFile || !kCurrentFile->IsOpen() ) {
            std::cout << "[WARNING] Opening file: " << kCurrentFileName.Data() << " failed!" << std::endl;
            continue;
        }
        //! Load Tree
        ALCORout    kCurrentData;
        TTree*      kCurrentTree = (TTree *)kCurrentFile->Get("alcor");
        if ( !kCurrentTree ) {
            std::cout << "[WARNING] No \"alcor\" tree in file: " << kCurrentFileName.Data() << ". Loading failed!" << std::endl;
            continue;
        }
        //! Get Entries and loop to fill histogram
        auto nEvents = kCurrentTree->GetEntries();
        LoadTree( kCurrentTree, kCurrentData );
        //!
        //! kFine\_All\_Tune\_Params con i dati calibrati
        for (int iEv = 0; iEv < nEvents; ++iEv) {
            kCurrentTree->GetEntry(iEv);
            int     iCurrentIndex   = uGetIndex( kCurrentData.fifo, kCurrentData.pixel, kCurrentData.column, kCurrentData.tdc );
            if ( iCurrentIndex < 0 ) {
                std::cout << "[WARNING] Invalid global index " << iCurrentIndex << " found! Skipping..." << std::endl;
                continue;
            }
            float   kCurrentMaximum = kFine_All_Tune_Params->GetBinContent( iCurrentIndex+1, 2 );
            float   kCurrentMinimum = kFine_All_Tune_Params->GetBinContent( iCurrentIndex+1, 4 );
            hFine_All_Tuned->Fill( iCurrentIndex, (kCurrentData.fine-kCurrentMinimum) );
        }
        kCurrentFile->Close();
    }
    //! Save output
    if ( !(kOutputFileName.Length() == 0) ) {
        TFile*  kFileOut = new TFile( kOutputFileName, "RECREATE" );
        kFine_All_Tune_Params->Write();
        hFine_All_Tuned->Write();
        kFileOut->Close();
    }
    //! Run in Batch mode
    gROOT->SetBatch(false);
    return hFine_All_Tuned;
}
#endif
