#ifndef ProgramOptions_h
#define ProgramOptions_h

#include <string>

class ProgramOptions{

 public:

  ProgramOptions(int argc, char* argv[]);
  ~ProgramOptions();

  std::string iDir;
  std::string oDir;
  std::string datasetFile;
  //double lumi;
  //int qcdMethod;
  //int wTauMethod;
  //bool doMCFMWeights;
  //std::string leptCorr;
  
};

#endif
