#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);
   
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   std::cout << "Printing EventFilter info from Metadata \n\n";
            
   FilterResults evtFilter = analysis.eventFilter("MssmHbb/Metadata/EventFilter");    
   
   std::cout << "Total    events   =  " << evtFilter.total      << std::endl;
   std::cout << "Filtered events   =  " << evtFilter.filtered   << std::endl;
   std::cout << "Filter efficiency =  " << evtFilter.efficiency << std::endl;
     
//    
}

