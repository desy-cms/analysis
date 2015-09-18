#ifndef Analysis_Tools_Analysis_h
#define Analysis_Tools_Analysis_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Analysis
// 
/**\class Analysis Analysis.cc Analysis/Tools/src/Analysis.cc

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
#include <vector>
#include <string>
// 
// user include files

#include "TChain.h"
#include "TFileCollection.h"
#include "Analysis/Tools/interface/Utils.h"

#include "Analysis/Tools/interface/Jets.h"
#include "Analysis/Tools/interface/Muons.h"
#include "Analysis/Tools/interface/METs.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Analysis {
         public:
            Analysis(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~Analysis();
           
            // Event
            int NumberOfEvents();
            void Event(const int & i);
            int Event();
            int Run();
            int LumiSection();
            void AddPhysicsObject(const std::string & unique_name, const std::string & path , const std::string & type = "" );
            TChain * PhysicsObject(const std::string & unique_name);
           
            // Cross sections
            void   CrossSections(const std::string & path);
            double CrossSection();
            double CrossSection(const std::string & title);
            void   ListCrossSections();
            
            // Generator Filter
            FilterResults GeneratorFilter(const std::string & path);
            void ListGeneratorFilter();
           
            // ----------member data ---------------------------
         protected:
               
            TFileCollection * fileCollection_;
            TCollection * fileList_;
            std::string inputFilelist_;
            
            
         private:

            std::map<std::string, double> xsections_;
            FilterResults genfilter_;
            
            int event_;
            int run_;
            int lumi_;
            
            int nevents_;
            
            // TREES
            TChain * t_xsection_;
            TChain * t_genfilter_;
            TChain * t_event_;
            
            std::map<std::string, TChain*> t_physobjs_;
            

      };
   }
}

#endif  // Analysis_Tools_Analysis_h
