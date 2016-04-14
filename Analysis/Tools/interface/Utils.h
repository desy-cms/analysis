#ifndef Analysis_Tools_Utils_h
#define Analysis_Tools_Utils_h 1

namespace analysis {
   namespace tools {

      struct FilterResults
      {
         int   total;
         int   filtered;
         float efficiency;
      };
      
      struct PDF
      {
         std::pair<int,int> id;
         std::pair<double,double> x;
      };
   }
}

#endif  // Analysis_Tools_Utils_h
