#ifndef Analysis_BackgroundModel_HistContainer_h
#define Analysis_BackgroundModel_HistContainer_h 1

#include <string>
#include <vector>
#include <memory>
#include "TH1.h"


namespace analysis {
  namespace backgroundmodel {

    class HistContainer {
    public:
      HistContainer(const std::string& input);
      virtual ~HistContainer();

      std::unique_ptr<TH1> data() const;
      std::unique_ptr<TH1> bbH() const;
      std::unique_ptr<TH1> background() const;
      std::vector<std::unique_ptr<TH1> > backgrounds() const;
      void show() const;
      void Rebin(const int& n = 1) const;

    private:
      std::unique_ptr<TH1> getHistogram_(const std::string& name) const;
      static std::unique_ptr<TH1> uniqueClone_(const TH1& original);

      const std::string histFileName_;
      std::unique_ptr<TH1> data_;
      std::unique_ptr<TH1> bbH_;
      std::vector<std::unique_ptr<TH1> > backgroundTemplates_;
      std::unique_ptr<TH1> summedBackground_;
    };

  }
}

#endif  // Analysis_BackgroundModel_HistContainer_h
