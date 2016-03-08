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
#include "Analysis/Tools/interface/GenParticle.h"
//
// class declaration
//

namespace analysis {
   namespace tools {
      
      class Jet : public Candidate {
         public:
            /// default constructor
            Jet();
            /// constructor from 4-momentum information
            Jet(const float & pt, const float & eta, const float & phi, const float & e);
            /// destructor
           ~Jet();
           
            // Gets
            /// returns the btag value of btag_csvivf
            float btag()                        const;
            /// returns the flavour with the Hadron definition (=0 for data)
            int   flavour()                     const;
            /// returns the flavour given a definition (=0 for data)
            int   flavour(const std::string & ) const;
            /// returns if jet has id loose working point
            bool  idLoose()                     const;
            /// returns if jet has id tight working point
            bool  idTight()                     const;
            /// returns the jet energy correction uncertainty
            float jecUncert()                   const;
            /// returns the vector containing flavours inside the jet
            std::vector<int> flavours()         const;
            /// returns the extended flavour definition
            std::string extendedFlavour()       const;
            /// returns the vector of pointers to the generated partons
            std::vector< std::shared_ptr<GenParticle> > partons() const;
            
            // Sets
            /// sets the btag value
            void  btag(const float &);
            /// sets flavour
            void  flavour(const int &);
            /// sets flavour for a given definition
            void  flavour(const std::string & definition, const int & value);
            /// sets if jet id is loose or not
            void  idLoose(const bool &);
            /// sets if jet id is tight or not
            void  idTight(const bool &);
            /// sets the jet energy correction uncertainty
            void  jecUncert(const float &);
            
            /// add parton that gave rise to jet
            void addParton(const std::shared_ptr<GenParticle> &);
            /// remove parton from jet parton list
            int removeParton(const int &);
            
            /// calculates the jet id
            void id(const float & nHadFrac,
                    const float & nEmFrac ,
                    const float & nMult   ,
                    const float & cHadFrac,
                    const float & cEmFrac ,
                    const float & cMult   ,
                    const float & muFrac  );
            
            /// associate partons to the jet
            void associatePartons(const std::vector< std::shared_ptr<GenParticle> > &, const float & dRmax = 0.5, const float & ptMin = 1., const bool & pythi8 = true );
            
//            using Candidate::set; // in case needed to overload the function set
            
         protected:
            // ----------member data ---------------------------
            //
            /// btag value 
            float btag_ ;
            /// map of flavour to a given definition
            std::map<std::string, int> flavour_;
            /// flavours inside the jet
            std::vector<int> flavours_;
            /// extended flavour identification for merged jets
            std::string extendedFlavour_;
            /// vector of pointers to Genparticles from merged jets
            std::vector< std::shared_ptr<GenParticle> > partons_;
            /// jet id loose working point
            bool  idloose_;
            /// jet id tight working point
            bool  idtight_;
            /// jet energy correction uncertainty
            float jecUnc_;
         private:
            // ----------member data ---------------------------
            
      };
   }
}

#endif  // Analysis_Tools_Jet_h
