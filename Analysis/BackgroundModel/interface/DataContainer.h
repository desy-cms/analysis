#ifndef Analysis_BackgroundModel_DataContainer_h
#define Analysis_BackgroundModel_DataContainer_h 1

#include <string>
#include <vector>
#include <memory>
#include "TH1.h"
#include "TFile.h"


namespace analysis {
  namespace backgroundmodel {

    class DataContainer {
    public:
      DataContainer(const std::string& input);
      virtual ~DataContainer();

      std::shared_ptr<TH1> data() const;
      std::shared_ptr<TH1> bbH() const;
      std::shared_ptr<TH1> background() const;
      void show() const;

    private:
      std::unique_ptr<TH1> getHistogram_(const std::string& name);

      static unsigned int dataCount_;
      static unsigned int bbHCount_;
      static unsigned int backgroundCount_;

      const std::string histFileName_;
      std::unique_ptr<TH1> data_;
      std::unique_ptr<TH1> bbH_;
      std::vector<std::unique_ptr<TH1> > backgroundTemplates_;
      std::unique_ptr<TH1> summedBackground_;
    };

  }
}

#endif  // Analysis_BackgroundModel_DataContainer_h
