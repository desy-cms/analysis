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
           
            float btag()                                  const;
            int   flavour()                               const;
            int   flavour(const std::string & definition) const;
            bool  idLoose()                               const;;
            bool  idTight()                               const;;
            float jecUncert()                             const;;
            
            void btag(const float &);
            void flavour(const int &);
            void flavour(const std::string & definition, const int & value);
            void idLoose(const bool &);
            void idTight(const bool &);
            void jecUncert(const float &);
            
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
            std::map<std::string, int> flavour_;
            bool  idloose_;
            bool  idtight_;
            float jecUnc_;
         private:
            // ----------member data ---------------------------
            
      };
         
      // ===============================================
      // INLINE IMPLEMENTATIONS
         
      // Gets
      inline float Jet::btag()                                  const { return btag_;                   }                   
      inline int   Jet::flavour()                               const { return flavour_.at("Hadron");   }                   
      inline int   Jet::flavour(const std::string & definition) const { return flavour_.at(definition); }                   
      inline bool  Jet::idLoose()                               const { return idloose_;                }                   
      inline bool  Jet::idTight()                               const { return idtight_;                }         
      inline float Jet::jecUncert()                             const { return jecUnc_;                 }                   
                
      // Sets                                                             
      inline void Jet::btag     (const float & btag)                               { btag_    = btag; } 
      inline void Jet::flavour  (const int   & flav)                               { flavour_["Hadron"] = flav; } 
      inline void Jet::flavour  (const std::string & definition, const int & flav) { flavour_[definition] = flav; } 
      inline void Jet::idLoose  (const bool  & loos)                               { idloose_ = loos; } 
      inline void Jet::idTight  (const bool  & tigh)                               { idtight_ = tigh; } 
      inline void Jet::jecUncert(const float & ju)                                 { jecUnc_  = ju; } 
   }
}

#endif  // Analysis_Tools_Jet_h
