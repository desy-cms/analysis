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
            Jet(const float & pt, const float & eta, const float & phi, const float & e);
           ~Jet();
           
            float btag() const;
            int   flavour();
            int   flavour(const std::string & definition = "Hadron");
            bool  idLoose();
            bool  idTight();
            
            void btag(const float &);
            void flavour(const int &);
            void flavour(const std::string & definition, const int & value);
            void idLoose(const bool &);
            void idTight(const bool &);
            
            void id(const float & nHadFrac,
                    const float & nEmFrac ,
                    const float & nMult   ,
                    const float & cHadFrac,
                    const float & cEmFrac ,
                    const float & cMult   ,
                    const float & muFrac  );
            
//            using Candidate::set; // in case needed to overload the function set
            
         protected:
            // ----------member data ---------------------------
            // 
            float btag_ ;
//            int   flavour_;
            std::map<std::string, int> flavour_;
            bool  idloose_;
            bool  idtight_;
         private:
            // ----------member data ---------------------------
            
      };
         
      // ===============================================
      // INLINE IMPLEMENTATIONS
         
      // Gets
      inline float Jet::btag()    const { return btag_;    }                   
      inline int   Jet::flavour() { return flavour_["Hadron"]; }                   
      inline int   Jet::flavour(const std::string & definition) { return flavour_[definition]; }                   
      inline bool  Jet::idLoose() { return idloose_; }                   
      inline bool  Jet::idTight() { return idtight_; }         
                
      // Sets                                                             
      inline void Jet::btag    (const float & btag) { btag_    = btag; } 
      inline void Jet::flavour (const int   & flav) { flavour_["Hadron"] = flav; } 
      inline void Jet::flavour (const std::string & definition, const int   & flav) { flavour_[definition] = flav; } 
      inline void Jet::idLoose (const bool  & loos) { idloose_ = loos; } 
      inline void Jet::idTight (const bool  & tigh) { idtight_ = tigh; } 
   }
}

#endif  // Analysis_Tools_Jet_h
