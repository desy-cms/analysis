#ifndef MssmHbbAnalysis_Ntuplizer_Candidates_h
#define MssmHbbAnalysis_Ntuplizer_Candidates_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      Candidates
// 
/**\class Candidates Candidates.cc MssmHbbAnalysis/Ntuplizer/src/Candidates.cc

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
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {

      template <typename T>
      class Candidates {
         public:
            Candidates();
            Candidates(const edm::InputTag&, TTree*, const bool &, float minPt = -1., float maxEta = -1.);
           ~Candidates();
            void ReadFromEvent(const edm::Event&);
            void BTagAlgorithms(const std::vector<std::string> &, const std::vector<std::string> &);
            void Init(const std::vector<std::string> &btagAlgos = std::vector<std::string>(), const std::vector<std::string> & btagAlgosAlias = std::vector<std::string>()	);
            void Branches();
            void Fill(const edm::Event&);
            void Fill();
            void Kinematics();
            void MinPt(const float& minPt = -1.);
            void MaxEta(const float& maxEta = -1.);
            static const int maxCandidates = 100;
      
         protected:
            // ----------member data ---------------------------
            std::vector<T> candidates_;
            std::string configParameter_;
            edm::InputTag input_collection_;
            
            
            // particles kinematics for the ntuple
            int   n_;
            float eta_[maxCandidates];
            float phi_[maxCandidates];
            float pt_[maxCandidates];
            float px_[maxCandidates];
            float py_[maxCandidates];
            float pz_[maxCandidates];
            float e_[maxCandidates];
            float et_[maxCandidates];
            int   q_[maxCandidates];
            
            float btag_[15][maxCandidates];
            int   flavour_[maxCandidates];
            
            float jetid_[7][maxCandidates];
            
            int pdg_[maxCandidates];
            int status_[maxCandidates];
            bool higgs_dau_[maxCandidates];
            
            TTree * tree_;
            
         private:
            bool is_l1jet_;
            bool is_l1muon_;
            bool is_calojet_;
            bool is_pfjet_;
            bool is_patjet_;
            bool is_patmuon_;
            bool is_genjet_;
            bool is_genparticle_;
            bool is_trigobject_;
            bool is_mc_;
            bool do_kinematics_;
            bool do_generator_;
            
            float minPt_;
            float maxEta_;
            std::vector<std::string>  btagAlgos_;
            std::vector<std::string>  btagAlgosAlias_;
            
            std::vector<std::string>  filterLabels_;
                      
            int higgs_pdg_;
   
            
      };
      // for the function specialisation - can also be done in .cc (keeping this comment for reference)
//      template <> int Candidates<pat::TriggerObject>::ReadFromEvent(const edm::Event& event);
   }
}

#endif  // MssmHbbAnalysis_Ntuplizer_Candidates_h
