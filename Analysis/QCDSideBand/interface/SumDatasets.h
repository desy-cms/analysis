#include "TH1D.h"
#include "TFile.h"

#include <string>
#include <vector>

class SumDatasets {
 public:

  SumDatasets(std::string dir, 
	      std::vector<std::string> datasets, 
	      std::vector<std::string> hists, 
	      std::string outfile);
  ~SumDatasets();

  void sumTH1D(TH1D* h, std::string label, std::vector<TFile*> files);

 private:

};
