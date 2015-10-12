#ifndef Analysis_BackgroundModel_DataContainer_h
#define Analysis_BackgroundModel_DataContainer_h 1

#include <string>
#include <vector>
#include "TH1.h"
#include "TFile.h"


namespace analysis {
  namespace backgroundmodel {

    class DataContainer {
    public:
      DataContainer(const std::string& input);
      virtual ~DataContainer();

      inline TH1* data() { return data_; };
      inline TH1* bbH() { return bbH_; };
      inline TH1* background() { return summedBackgroundTemplates_; };

    private:
      TH1* getHistogram_(const std::string& name);

      const std::string histFileName_;
      TH1* data_;
      TH1* bbH_;
      std::vector<TH1*> backgroundTemplates_;
      TH1* summedBackgroundTemplates_;
    };

  }
}

#endif  // Analysis_BackgroundModel_DataContainer_h
