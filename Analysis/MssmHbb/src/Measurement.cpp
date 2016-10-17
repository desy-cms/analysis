#include "Analysis/MssmHbb/interface/Measurement.h"

Measurement::Measurement() : median_(0), plus1G_(0), minus1G_(0), plus2G_(0), minus2G_(0){};

Measurement::Measurement(const double& median) : median_(median), plus1G_(0), minus1G_(0), plus2G_(0), minus2G_(0) {};

Measurement::Measurement(const double& median, const double& err) : median_(median), plus1G_(median+err), minus1G_(median-err), plus2G_(median+2*err), minus2G_(median-2*err) {};

Measurement::Measurement(const double& median, const double& plus1G, const double& minus1G, const double& plus2G, const double& minus2G) :
		median_(median), plus1G_(plus1G), minus1G_(minus1G),  plus2G_(plus2G), minus2G_(minus2G) {};

Measurement::Measurement(const double& median, const double& plus1G, const double& minus1G) : median_(median), plus1G_(plus1G), minus1G_(minus1G){
	plus2G_ = median + (plus1G - median)*2;
	minus2G_ = median + (minus1G - median)*2;
};

double Measurement::getMedian() const {
	return median_;
}

void Measurement::setMedian(const double& median) {
	median_ = median;
}

double Measurement::getMinus1G() const {
	return minus1G_;
}

void Measurement::setMinus1G(const double& minus1G) {
	minus1G_ = minus1G;
}

double Measurement::getMinus2G() const {
	return minus2G_;
}

void Measurement::setMinus2G(const double& minus2G) {
	minus2G_ = minus2G;
}

double Measurement::getPlus1G() const {
	return plus1G_;
}

void Measurement::setPlus1G(const double& plus1G) {
	plus1G_ = plus1G;
}

double Measurement::getPlus2G() const {
	return plus2G_;
}

void Measurement::setPlus2G(const double& plus2G) {
	plus2G_ = plus2G;
}
