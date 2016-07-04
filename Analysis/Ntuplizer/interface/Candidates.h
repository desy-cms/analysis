#ifndef Analysis_Ntuplizer_Candidates_h
#define Analysis_Ntuplizer_Candidates_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Candidates
// 
/**\class Candidates Candidates.cc Analysis/Ntuplizer/src/Candidates.cc

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
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Analysis/Ntuplizer/interface/Utils.h"

#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace ntuple {

      template <typename T>
      class Candidates {
         public:
            Candidates();
            Candidates(const edm::InputTag&, TTree*, const bool &, float minPt = -1., float maxEta = -1.);
           ~Candidates();
            void ReadFromEvent(const edm::Event&);
            void BTagAlgorithms(const std::vector<std::string> &, const std::vector<std::string> &);
//            void Init(const std::vector<std::string> &btagAlgos = std::vector<std::string>(), const std::vector<std::string> & btagAlgosAlias = std::vector<std::string>()	);
//            void Init(const std::map<std::string, TitleIndex> & btagVars = std::map<std::string,TitleIndex>()	);
//            void Init(const std::vector<TitleAlias> & btagVars = std::vector<TitleAlias>()	);
            void Init();
            void Init(const std::vector<TitleAlias> &	);
//             void Init(const std::vector<TitleAlias> & , const std::string &	);
//             void Init(const std::vector<TitleAlias> & , const std::string & , const std::string & , const edm::InputTag & );
//             void Init(const std::vector<TitleAlias> & , const std::string & , const std::string & , const std::string & , const std::string &, const edm::InputTag & );
            void AddJecInfo(const std::string & );
            void AddJecInfo(const std::string &, const std::string & );
            void AddJerInfo(const std::string &, const edm::InputTag & );
            void AddJerInfo(const std::string &, const std::string &, const std::string &, const edm::InputTag &  );
            void Branches();
            void Fill(const edm::Event&);
            void Fill(const edm::Event&, const edm::EventSetup&);
            void Fill();
            void Kinematics();
            void MinPt(const float& minPt = -1.);
            void MaxEta(const float& maxEta = -1.);
            void JECRecord(const std::string &);
            static const int maxCandidates = 200;
      
         protected:
            // ----------member data ---------------------------
            std::vector<T> candidates_;
            std::string configParameter_;
            edm::InputTag input_collection_;
            
            std::string jecRecord_;
            std::string jecFile_;
            std::unique_ptr<JetCorrectionUncertainty> jecUnc_;
            std::string jerRecord_;
            std::string jerFile_;
            std::string jersfFile_;

            
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
            int   hadronFlavour_[maxCandidates];
            int   partonFlavour_[maxCandidates];
            int   physicsFlavour_[maxCandidates];
            
            float jetid_[15][maxCandidates];
            
            // Jet energy resolution and scale correction
            float jecUncert_[maxCandidates];
            edm::InputTag rho_collection_;
            double rho_;
            float jerResolution_[maxCandidates];
            float jerSF_[maxCandidates];
            float jerSFUp_[maxCandidates];
            float jerSFDown_[maxCandidates];            
           	JME::JetResolution res_;
           	JME::JetResolutionScaleFactor res_sf_;
                        
            int pdg_[maxCandidates];
            int status_[maxCandidates];
            bool lastcopy_[maxCandidates];
            bool higgs_dau_[maxCandidates];
            
            // met specifics
            float sigxx_[maxCandidates];
            float sigxy_[maxCandidates];
            float sigyx_[maxCandidates];
            float sigyy_[maxCandidates];
            
            // gen info (usually from pat objects)
            float gen_px_[maxCandidates];
            float gen_py_[maxCandidates];
            float gen_pz_[maxCandidates];
            
            // type
            int  type_[maxCandidates];

            
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
            bool is_patmet_;
            bool is_mc_;
            bool do_kinematics_;
            bool do_generator_;
            
            float minPt_;
            float maxEta_;
            std::vector<std::string>  btagAlgos_;
            std::vector<std::string>  btagAlgosAlias_;
            
            std::vector<std::string>  filterLabels_;
                      
            int higgs_pdg_;
            
            std::vector<TitleAlias>  id_vars_;
            std::vector<TitleAlias>  btag_vars_;
   
            
      };
      // for the function specialisation - can also be done in .cc (keeping this comment for reference)
//      template <> int Candidates<pat::TriggerObject>::ReadFromEvent(const edm::Event& event);
   }
}

#endif  // Analysis_Ntuplizer_Candidates_h
