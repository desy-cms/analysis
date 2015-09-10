#ifndef Analysis_Ntuplizer_Utils_h
#define Analysis_Ntuplizer_Utils_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Utils
// 
/**\class Utils Utils.cc Analysis/Ntuplizer/src/Utils.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include <string>
// 
// user include files
//
// class declaration
//

namespace analysis {
   namespace ntuple {
      
      struct TitleIndex {
         std::string title;
         unsigned int index;
      };
      struct TitleAlias {
         std::string title;
         std::string alias;
      };
   }
}   

#endif  // Analysis_Ntuplizer_Utils_h
