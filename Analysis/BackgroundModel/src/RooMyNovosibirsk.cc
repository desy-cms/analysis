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

double RooMyNovosibirsk::evaluate() const
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

