#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooMyNovosibirsk.h"


using namespace analysis::backgroundmodel;


ClassImp(RooMyNovosibirsk)


RooMyNovosibirsk::RooMyNovosibirsk(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& peak,
                             RooAbsReal& width,
                             RooAbsReal& tail) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  peak_("peak", "peak", this, peak),
  width_("width", "width", this, width),
  tail_("tail", "tail", this, tail) {
}


RooMyNovosibirsk::RooMyNovosibirsk(const RooMyNovosibirsk& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  peak_("peak", this, other.peak_),
  width_("width", this, other.width_),
  tail_("tail", this, other.tail_) {
}


TObject* RooMyNovosibirsk::clone(const char* newname) const {
  return new RooMyNovosibirsk(*this, newname);
}

/*
double RooMyNovosibirsk::evaluate() const {

  double lambda = TMath::SinH(tail_ * TMath::Sqrt(TMath::Log(4))) / (width_ * tail_ * TMath::Sqrt(TMath::Log(4)));
  double result = 0.0;

  if(TMath::Abs(tail_) < 1.e-07){
	result = TMath::Exp(-0.5 * TMath::Power((x_ - peak_)/width_,2.));
  }
  else if ((1 + lambda * tail_* (x_ - peak_)) > 0){
  	double theLog = TMath::Log(1 + lambda * tail_ * (x_ - peak_));
  	result = TMath::Exp(-0.5 * (theLog * theLog / (tail_ * tail_) + tail_ * tail_));
  }
  else result = 0;
          
  return result;

}
*/

Double_t RooMyNovosibirsk::evaluate() const
{ 
  if (TMath::Abs(tail_) < 1.e-7) {
     return TMath::Exp( -0.5 * TMath::Power( ( (x_ - peak_) / width_ ), 2 ));
  }
   
  double arg = 1.0 - ( x_ - peak_ ) * tail_ / width_;
   
  if (arg < 1.e-7) {
     //Argument of logaritem negative. Real continuation -> function equals zero
     return 0.0;
  }
     
  double log = TMath::Log(arg);    
  static const double xi = 2.3548200450309494; // 2 Sqrt( Ln(4) )
     
  double width_zero = ( 2.0 / xi ) * TMath::ASinH( tail_ * xi * 0.5 );
  double width_zero2 = width_zero * width_zero;
  double exponent = ( -0.5 / (width_zero2) * log * log ) - ( width_zero2 * 0.5 );
     
  return TMath::Exp(exponent) ;
}

/*
Int_t RooNovosibirsk::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* ) const
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  if (matchArgs(allVars,analVars,peak)) return 2 ;
     
  //The other two integrals over tali and width are not integrable
     
  return 0 ;
}

Double_t RooNovosibirsk::analyticalIntegral(Int_t code, const char* rangeName) const
{
  assert(code==1 || code==2) ;
  
  //The range is defined as [A,B]
 
  //Numerical values need for the evaluation of the integral
  static const Double_t sqrt2 = 1.4142135623730950; // Sqrt(2)
  static const Double_t sqlog2 = 0.832554611157697756; //Sqrt( Log(2) )
  static const Double_t sqlog4 = 1.17741002251547469; //Sqrt( Log(4) )
  static const Double_t log4 = 1.38629436111989062; //Log(2)
  static const Double_t rootpiby2 = 1.2533141373155003; // Sqrt(pi/2)
  static const Double_t sqpibylog2 = 2.12893403886245236; //Sqrt( pi/Log(2) )
    
  if (code==1) {
    Double_t A = x.min(rangeName);
    Double_t B = x.max(rangeName);
        
    Double_t result = 0;

        
    //If tail==0 the function becomes gaussian, thus we return a gassian integral
    if (TMath::Abs(tail) < 1.e-7) {

      Double_t xscale = sqrt2*width;
            
      result = rootpiby2*width*(TMath::Erf((B-peak)/xscale)-TMath::Erf((A-peak)/xscale));
            
      return result;
            
    }
        
    // If the range is not defined correctly the function becomes complex
    Double_t log_argument_A = ( (peak - A)*tail + width ) / width ;
    Double_t log_argument_B = ( (peak - B)*tail + width ) / width ;
        
    //lower limit
    if ( log_argument_A < 1.e-7) {
      log_argument_A = 1.e-7;
    }
        
    //upper limit
    if ( log_argument_B < 1.e-7) {
      log_argument_B = 1.e-7;
    }
        
    Double_t term1 =  TMath::ASinH( tail * sqlog4 );
    Double_t term1_2 =  term1 * term1;
        
    //Calculate the error function arguments
    Double_t erf_termA = ( term1_2 - log4 * TMath::Log( log_argument_A ) ) / ( 2 * term1 * sqlog2 );
    Double_t erf_termB = ( term1_2 - log4 * TMath::Log( log_argument_B ) ) / ( 2 * term1 * sqlog2 );
        
    result = 0.5 / tail * width * term1 * ( TMath::Erf(erf_termB) - TMath::Erf(erf_termA)) * sqpibylog2;
        
    return result;
        
  } else if (code==2) {
    Double_t A = x.min(rangeName);
    Double_t B = x.max(rangeName);
        
    Double_t result = 0;
        
        
    //If tail==0 the function becomes gaussian, thus we return a gassian integral
    if (TMath::Abs(tail) < 1.e-7) {
            
      Double_t xscale = sqrt2*width;
            
      result = rootpiby2*width*(TMath::Erf((B-x)/xscale)-TMath::Erf((A-x)/xscale));
            
      return result;
            
    }
        
    // If the range is not defined correctly the function becomes complex
    Double_t log_argument_A = ( (A - x)*tail + width ) / width;
    Double_t log_argument_B = ( (B - x)*tail + width ) / width;
        
    //lower limit
    if ( log_argument_A < 1.e-7) {
      log_argument_A = 1.e-7;
    }
        
    //upper limit
    if ( log_argument_B < 1.e-7) {
      log_argument_B = 1.e-7;
    }
        
    Double_t term1 =  TMath::ASinH( tail * sqlog4 );
    Double_t term1_2 =  term1 * term1;
        
    //Calculate the error function arguments
    Double_t erf_termA = ( term1_2 - log4 * TMath::Log( log_argument_A ) ) / ( 2 * term1 * sqlog2 );
    Double_t erf_termB = ( term1_2 - log4 * TMath::Log( log_argument_B ) ) / ( 2 * term1 * sqlog2 );
        
    result = 0.5 / tail * width * term1 * ( TMath::Erf(erf_termB) - TMath::Erf(erf_termA)) * sqpibylog2;
        
    return result;
        
  }

  // Emit fatal error
  coutF(Eval) << "Error in RooNovosibirsk::analyticalIntegral" << std::endl;  

  // Put dummy return here to avoid compiler warnings
  return 1.0 ;
}
*/



