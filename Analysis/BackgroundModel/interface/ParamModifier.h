#ifndef Analysis_BackgroundModel_ParamModifier_h
#define Analysis_BackgroundModel_ParamModifier_h 1

#include <string>
#include "RooRealVar.h"


namespace analysis {
  namespace backgroundmodel {

    class ParamModifier {
    public:
      ParamModifier(const std::string& name);
      virtual ~ParamModifier() {};

      ParamModifier& start(float value);
      ParamModifier& min(float value);
      ParamModifier& max(float value);
      ParamModifier& constant();
      ParamModifier& floating();

      inline const std::string& name() const { return name_; };
      bool modify(RooRealVar& var) const;

    private:
      const std::string name_;
      bool constant_;
      bool floating_;
      float start_;
      float min_;
      float max_;
      bool changeStart_;
      bool changeMin_;
      bool changeMax_;
    };


    inline ParamModifier& ParamModifier::start(float value) {
      start_ = value;
      changeStart_ = true;
      return *this;
    }


    inline ParamModifier& ParamModifier::min(float value) {
      min_ = value;
      changeMin_ = true;
      return *this;
    }


    inline ParamModifier& ParamModifier::max(float value) {
      max_ = value;
      changeMax_ = true;
      return *this;
    }


    inline ParamModifier& ParamModifier::constant() {
      constant_ = true;
      floating_ = false;
      return *this;
    }


    inline ParamModifier& ParamModifier::floating() {
      floating_ = true;
      constant_ = false;
      return *this;
    }

  }
}

#endif  // Analysis_BackgroundModel_ParamModifier_h
