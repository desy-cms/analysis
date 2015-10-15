#ifndef Analysis_BackgroundModel_Tools_h
#define Analysis_BackgroundModel_Tools_h 1

#include <vector>
#include <string>
#include <sstream>
#include "Analysis/BackgroundModel/interface/ParamModifier.h"


namespace analysis {
  namespace backgroundmodel {

    std::vector<ParamModifier> parseModifiers(const std::vector<std::string>& input);
    
    template <typename T>
    std::string NumToStr(T num) {
      std::stringstream ss;
      ss << num;
      return ss.str();
    };

    template <typename T>
    T StrToNum(const std::string& text) {
      std::stringstream ss(text);
      T result;
      return ss >> result ? result : 0;
    };


  }
}

#endif  // Analysis_BackgroundModel_Tools_h
