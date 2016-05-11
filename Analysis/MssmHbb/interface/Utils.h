#ifndef Analysis_MssmHbb_interface_Utils_h
#define Analysis_MssmHbb_interface_Utils_h 1

namespace analysis {
   namespace tools {

      struct SignalEfficiency
      {
    	  SignalEfficiency(double &m, double & sel, double & tot ){
    		  total = tot;
    		  selected = sel;
    		  mass = m;
    		  efficiency = selected / total;
    	  }

         double   	total;
         double   	selected;
         double 	efficiency;
         double   	mass;
      };
      
   }
}

#endif  // Analysis_MssmHbb_interface_Utils_h
