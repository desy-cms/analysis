#include <cmath>
#include "Riostream.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooRelBreitWigner.h"


using namespace analysis::backgroundmodel;


ClassImp(RooRelBreitWigner)


RooRelBreitWigner::RooRelBreitWigner(const char *name, const char *title,
                         RooAbsReal& x,
                         RooAbsReal& peak,
                         RooAbsReal& width) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  peak_("peak", "peak", this, peak),
  width_("width", "width", this, width) {
}


RooRelBreitWigner::RooRelBreitWigner(const RooRelBreitWigner& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  peak_("peak", this, other.peak_),
  width_("width", this, other.width_) {
}


TObject* RooRelBreitWigner::clone(const char* newname) const {
  return new RooRelBreitWigner(*this, newname);
}


double RooRelBreitWigner::evaluate() const {
  double M  = x_;
  double M2 = M*M;
  double G = width_;
  double G2 = G*G;
  double E = peak_;
  double E2= E*E;
  double gamma = std::sqrt(M2 * (M2 + G2));
  double k = (2 * std::sqrt(2) * M * G * gamma) / (TMath::Pi() * std::sqrt(M2 + gamma));
  double result = k / ( std::pow(E2-M2,2.)+ M2*G2 );
//  double result = 2 / TMath::Pi() * G2*E2 / ( (M2-E2)*(M2-E2)+ M2*M2*(G2/E2) );

  return result;
}



