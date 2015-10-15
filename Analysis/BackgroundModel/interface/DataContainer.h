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

      TH1* data() const;
      TH1* bbH() const;
      TH1* background() const;
      void show() const;

    private:
      TH1* getHistogram_(const std::string& name);

      static unsigned int dataCount_;
      static unsigned int bbHCount_;
      static unsigned int backgroundCount_;

      const std::string histFileName_;
      TH1* data_;
      TH1* bbH_;
      std::vector<TH1*> backgroundTemplates_;
      TH1* summedBackground_;
    };

  }
}

#endif  // Analysis_BackgroundModel_DataContainer_h
