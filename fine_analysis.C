
int get_index(int fifo, int pixel, int column, int tdc);
TH2 *fine_fill(std::vector<std::string> filenames);
TH1 *fine_histo(TH2 *hFine_all, int index);

TF1 *double_fermi_function();

struct data_t {
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

enum type_t {
  kAlcorHit = 1,
  kTriggerTag = 9,
  kStartSpill = 7,
  kEndSpill = 15
};

/*******************************************************************************/

int
get_index(int fifo, int pixel, int column, int tdc)
{
  /**
     4 TDCs for each pixel
     4 pixel for each column
     8 columns for each chip
  **/
  int chip = fifo / 4;
  int index = tdc + 4 * pixel + 16 * column + 128 * chip;
  return index;
}

/*******************************************************************************/

TH2 *
fine_fill(std::vector<std::string> filenames)
{

  /** histogram with fine distribution **/
  auto hFine_all = new TH2F("hFine_all", ";index;fine", 768, 0, 768, 512, 0, 512);

  /** loop over input file list **/
  for (const auto filename : filenames) {

    /** open file **/
    std::cout << " --- opening decoded file: " << filename << std::endl; 
    auto fin = TFile::Open(filename.c_str());
    if (!fin || !fin->IsOpen()) continue;

    /** retrieve tree and link it **/
    data_t data;
    auto tin = (TTree *)fin->Get("alcor");
    auto nev = tin->GetEntries();
    std::cout << " --- found " << nev << " entries in tree " << std::endl;
    tin->SetBranchAddress("fifo", &data.fifo);
    tin->SetBranchAddress("type", &data.type);
    tin->SetBranchAddress("counter", &data.counter);
    tin->SetBranchAddress("column", &data.column);
    tin->SetBranchAddress("pixel", &data.pixel);
    tin->SetBranchAddress("tdc", &data.tdc);
    tin->SetBranchAddress("rollover", &data.rollover);
    tin->SetBranchAddress("coarse", &data.coarse);
    tin->SetBranchAddress("fine", &data.fine);

    /** loop over events in tree **/
    for (int iev = 0; iev < nev; ++iev) {
      tin->GetEntry(iev);
      /** only ALCOR hits **/
      if (data.type != kAlcorHit) continue;
      int index = get_index(data.fifo, data.pixel, data.column, data.tdc);
      hFine_all->Fill(index, data.fine);
    }
    fin->Close();
  }
    
  return hFine_all;
}

/*******************************************************************************/

TH1 *
fine_histo(TH2 *hFine_all, int index)
{
  std::string name = "hFine_" + std::to_string(index);
  TH1 *hFine = hFine_all->ProjectionY(name.c_str(), index + 1, index + 1);
  return hFine;
}

/*******************************************************************************/

  TF1 *double_fermi_function()
{
  auto f = new TF1("f_fermi", "[0] * ( 1. / ( exp ( (x - [1]) / [2] ) + 1 ) ) * ( 1. / ( exp ( ([3] - x) / [4] ) + 1 ) )", 20., 100.);
  f->SetParameter(0, 1.);
  f->SetParameter(1, 256);
  f->SetParameter(2, 18.);
  f->SetParameter(3, 128);
  f->SetParameter(4, 2.);
  return f;

}

/**
   example of usage:
   root [0] .L fine_analysis.C 
   root [1] TH2 *hFine_all = fine_fill({"alcdaq.fifo_0.root", "alcdaq.fifo_1.root", "alcdaq.fifo_2.root", 		"alcdaq.fifo_3.root","alcdaq.fifo_4.root","alcdaq.fifo_5.root","alcdaq.fifo_6.root", "alcdaq.fifo_7.root", "alcdaq.fifo_8.root", 		"alcdaq.fifo_9.root", "alcdaq.fifo_10.root", "alcdaq.fifo_11.root", "alcdaq.fifo_12.root", 		"alcdaq.fifo_13.root","alcdaq.fifo_14.root", "alcdaq.fifo_15.root", "alcdaq.fifo_16.root","alcdaq.fifo_17.root", 		"alcdaq.fifo_18.root", "alcdaq.fifo_19.root", "alcdaq.fifo_20.root", "alcdaq.fifo_21.root", "alcdaq.fifo_22.root",     	  		"alcdaq.fifo_23.root"});
--- opening decoded file: alcdaq.fifo_0.root
 --- found 2151601 entries in tree 
 --- opening decoded file: alcdaq.fifo_1.root
 --- found 1875405 entries in tree 
 --- opening decoded file: alcdaq.fifo_2.root
 --- found 2105405 entries in tree 
 --- opening decoded file: alcdaq.fifo_3.root
 --- found 2461783 entries in tree 
 --- opening decoded file: alcdaq.fifo_4.root
 --- found 1366096 entries in tree 
 --- opening decoded file: alcdaq.fifo_5.root
 --- found 1323227 entries in tree 
 --- opening decoded file: alcdaq.fifo_6.root
 --- found 46 entries in tree 
 --- opening decoded file: alcdaq.fifo_7.root
 --- found 1225863 entries in tree 
 --- opening decoded file: alcdaq.fifo_8.root
 --- found 3191322 entries in tree 
 --- opening decoded file: alcdaq.fifo_9.root
 --- found 2609502 entries in tree 
 --- opening decoded file: alcdaq.fifo_10.root
 --- found 2373098 entries in tree 
 --- opening decoded file: alcdaq.fifo_11.root
 --- found 2886942 entries in tree 
 --- opening decoded file: alcdaq.fifo_12.root
 --- found 2498462 entries in tree 
 --- opening decoded file: alcdaq.fifo_13.root
 --- found 2589769 entries in tree 
 --- opening decoded file: alcdaq.fifo_14.root
 --- found 3830107 entries in tree 
 --- opening decoded file: alcdaq.fifo_15.root
 --- found 3149417 entries in tree 
 --- opening decoded file: alcdaq.fifo_16.root
 --- found 632255 entries in tree 
 --- opening decoded file: alcdaq.fifo_17.root
 --- found 629111 entries in tree 
 --- opening decoded file: alcdaq.fifo_18.root
 --- found 627284 entries in tree 
 --- opening decoded file: alcdaq.fifo_19.root
 --- found 633727 entries in tree 
 --- opening decoded file: alcdaq.fifo_20.root
 --- found 632143 entries in tree 
 --- opening decoded file: alcdaq.fifo_21.root
 --- found 633155 entries in tree 
 --- opening decoded file: alcdaq.fifo_22.root
 --- found 636310 entries in tree 
 --- opening decoded file: alcdaq.fifo_23.root
 --- found 635441 entries in tree  
   root [2] hFine_all->Draw("colz");
   root [3] TH1 *hFine_0 = fine_histo(hFine_all, 0);
   root [4] hFine_0->Draw();
   root [5] TF1 *ffermi = double_fermi_function()
   root [6] hFine_0->Fit(ffermi);
   root [7] TH1 *hFine_1 = fine_histo(hFine_all, 1);
   root [8] hFine_1->Fit(ffermi);
   ..etc fino a hFine_5...
 **/
