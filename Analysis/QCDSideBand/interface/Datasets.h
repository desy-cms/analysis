#ifndef Datasets_h
#define Datasets_h

#include "TFile.h"

#include <string>
#include <vector>

struct Dataset {
  std::string name;
  //int nEvents;
  //double sigma;
  bool isData;
};


class Datasets {

 public:

  Datasets(std::string iDir="");
  ~Datasets();

  void readFile(std::string filename);

  void addDataset(std::string name, 
		  //int nevt,
		  //double sigma,
		  int isData);

  unsigned size() { return names_.size(); }

  Dataset getDataset(unsigned i);

  Dataset getDataset(std::string name);

  TFile* getTFile(std::string name);

 private:

  std::vector<std::string> names_;
  //std::vector<int> nevts_;
  //std::vector<double> sigmas_;
  std::vector<bool> isData_;

  std::string idir_;

};

#endif
