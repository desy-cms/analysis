#include "Analysis/QCDSideBand/interface/SumDatasets.h"

#include <string>
#include <vector>
#include "TFile.h"
#include "TH1D.h"

#include <iostream>

void SumDatasets::sumTH1D(TH1D* h, std::string label, std::vector<TFile*> files) {
  std::vector<TFile*>::iterator file;
  for (file = files.begin(); file != files.end(); ++file) {
    if(file == files.begin()) continue;		// avoid double-counting histogram from the first file
    else h->Add( (TH1D*) (*file)->Get(label.c_str()) );
  }
}

SumDatasets::SumDatasets(std::string dir, 
			 std::vector<std::string> datasets, 
			 std::vector<std::string> hists, 
			 std::string outfile) {
 
  // open files
  std::vector<TFile*> files;
  std::vector<std::string>::const_iterator dataset;
  for (dataset=datasets.begin(); dataset!=datasets.end(); ++dataset) {

    TFile* file = TFile::Open( (dir+std::string("/")+(*dataset)+std::string(".root")).c_str(), "READ");

    //std::cout << "Dataset name is : " << (*dataset) << std::endl;
    if (file!=0) files.push_back( file );

  }
  //std::cout << "Number of file in files = " << files.size() << std::endl;

  TFile* ofile = TFile::Open( (dir+std::string("/")+outfile+std::string(".root")).c_str(), "RECREATE");

  //std::cout << "Successfully create file : " << ofile->GetName() << std::endl;

  // sum histograms
  std::vector<std::string>::const_iterator hist;
  for (hist=hists.begin(); hist!=hists.end(); ++hist) {

    TH1D* h( (TH1D*) files.at(0)->Get(hist->c_str()) );
    if (h==0) {
      std::cerr << "Could not find histogram " << (*hist) << " in " << ofile->GetName() << std::endl;
      continue;
    }
    sumTH1D(h, (*hist), files);

    //std::cout << "Entry in histogram : " << h->GetName() << " = " << h->GetEntries() << std::endl;
    h->Write();
  }



  // close files
  ofile->Close();

  std::vector<TFile*>::iterator file;
  for (file=files.begin(); file!=files.end(); ++file) {
    (*file)->Close();
  }


}


SumDatasets::~SumDatasets() {

}



 
