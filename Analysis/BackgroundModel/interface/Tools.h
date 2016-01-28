#ifndef Analysis_BackgroundModel_Tools_h
#define Analysis_BackgroundModel_Tools_h 1

#include <vector>
#include <memory>
#include <string>
#include "Analysis/BackgroundModel/interface/ParamModifier.h"


namespace analysis {
  namespace backgroundmodel {

    std::string uniquify(const std::string& original);
    TObject* uniqueClone(const TObject* original);
    std::vector<ParamModifier> parseModifiers(const std::vector<std::string>& input);

    template <typename T>
    std::unique_ptr<T> staticCastUnique(void* object) {
      return std::unique_ptr<T>(static_cast<T*>(object));
    };

  }
}

#endif  // Analysis_BackgroundModel_Tools_h
