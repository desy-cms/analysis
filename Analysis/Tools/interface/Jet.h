#ifndef Analysis_Tools_Jet_h
#define Analysis_Tools_Jet_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Jet
// 
/**\class Jet Jet.cc Analysis/Tools/src/Jet.cc

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
#include <memory>
// 
// user include files
#include "Analysis/Tools/interface/Candidate.h"
//
// class declaration
//

namespace analysis {
   namespace tools {

      class Jet : public Candidate {
         public:
            Jet();
           ~Jet();
            float btag();
            int   flavour();
            bool  idLoose();
            bool  idTight();
            
            void btag(const float &);
            void flavour(const int &);
            void idLoose(const bool &);
            void idTight(const bool &);
            
            using Candidate::set; // in case needed to overload the function set
      
         private:
            // ----------member data ---------------------------
            
            // 
            float btag_ ;
            int   flavour_;
            bool  idLoose_;
            bool  idTight_;
      };
   }
}

#endif  // Analysis_Tools_Jet_h
