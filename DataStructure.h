//  --- --- --- ---
//
//  Alcor readout utilities
//
//  Authors: Chiara Fraticelli, Nicola Rubini
//
//  --- --- --- ---
//  Constants
const Int_t kIndexRange = 768;
//
//  Data Structures
struct ALCORout {
  int fifo;
  int type;
  int counter;
  int column;
  int pixel;
  int tdc;
  int rollover;
  int coarse;
  int fine;
};
//
//  Reference Structures
enum ALCORtype {
  kAlcorHit = 1,
  kTriggerTag = 9,
  kStartSpill = 7,
  kEndSpill = 15
};
//
void
LoadTree
( TTree* kInputTree, ALCORout &kReadData ) {
    if ( !kInputTree ) { std::cout << "[ERROR] DataStructure::LoadTree for ALCORout: Invalid Tree given" << std::endl; return; }
    kInputTree->SetBranchAddress(   "fifo",     &kReadData.fifo     );
    kInputTree->SetBranchAddress(   "type",     &kReadData.type     );
    kInputTree->SetBranchAddress(   "counter",  &kReadData.counter  );
    kInputTree->SetBranchAddress(   "column",   &kReadData.column   );
    kInputTree->SetBranchAddress(   "pixel",    &kReadData.pixel    );
    kInputTree->SetBranchAddress(   "tdc",      &kReadData.tdc      );
    kInputTree->SetBranchAddress(   "rollover", &kReadData.rollover );
    kInputTree->SetBranchAddress(   "coarse",   &kReadData.coarse   );
    kInputTree->SetBranchAddress(   "fine",     &kReadData.fine     );
}
//

int
uGetIndex
( int fifo, int pixel, int column, int tdc, bool kUseFIFO = true ) {
    //! 4 TDCs for each pixel
    //! 4 pixel for each column
    //! 8 columns for each chip
    int chip = kUseFIFO ? fifo / 4 : fifo;
    int index = tdc + 4 * pixel + 16 * column + 128 * chip;
    return index;
}
std::tuple<Int_t,Int_t,Int_t,Int_t>
uGetInfos
( Int_t iIndex ) {
    //! Result { Chip, Pixel, Column, TDC }
    std::tuple<Int_t,Int_t,Int_t,Int_t> kResult = {-1,-1,-1,-1};
    get<0>(kResult) = (iIndex/128)%6;
    get<1>(kResult) = (iIndex/4)%4;
    get<2>(kResult) = (iIndex/16)%8;
    get<3>(kResult) = iIndex%4;
    return kResult;
}

