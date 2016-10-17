/*
 * Measurement.h
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_MEASUREMENT_H_
#define MSSMHBB_INTERFACE_MEASUREMENT_H_

class Measurement{
public:
	Measurement();
	Measurement(const double& median);
	Measurement(const double& median, const double& err);
	Measurement(const double& median, const double& plus1G, const double& minus1G);
	Measurement(const double& median, const double& plus1G, const double& minus1G, const double& plus2G, const double& minus2G);
	virtual ~Measurement() {};

	double getMedian() const;
	void setMedian(const double& median);
	double getMinus1G() const;
	void setMinus1G(const double& minus1G);
	double getMinus2G() const;
	void setMinus2G(const double& minus2G);
	double getPlus1G() const;
	void setPlus1G(const double& plus1G);
	double getPlus2G() const;
	void setPlus2G(const double& plus2G);

protected:

	double median_;
	double plus1G_;
	double minus1G_;
	double plus2G_;
	double minus2G_;

};

#endif
