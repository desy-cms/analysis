#ifndef MssmHbbAnalysis_Ntuplizer_Utils_h
#define MssmHbbAnalysis_Ntuplizer_Utils_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      Utils
// 
/**\class Utils Utils.cc MssmHbbAnalysis/Ntuplizer/src/Utils.cc

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

namespace mssmhbb {
   namespace ntuple {
      
      struct TitleIndex {
         std::string title;
         unsigned int index;
      };
   }
}   

#endif  // MssmHbbAnalysis_Ntuplizer_Utils_h
