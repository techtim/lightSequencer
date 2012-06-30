
#ifndef AUBIOONSETDETECTOR_H
#define AUBIOONSETDETECTOR_H

#include "aubio.h"

class AubioOnsetDetector
{
public:		
	AubioOnsetDetector();
	~AubioOnsetDetector();
	void initialise();
	bool processframe(float frame[], int n);
	
	//the following functions switch between different onset types
	void onsetclass_energy();	
	void onsetclass_complex();
	void onsetclass_specdiff();
	void onsetclass_kl();
	void onsetclass_mkl();
	void onsetclass_hfc();
	void onsetclass_phase();
		
	float getRawDetectionFrame();
	float getPeakPickedDetectionFrame();
//	boolean getIsOnset();
	
	aubio_onsetdetection_t		*o;
	aubio_pvoc_t				*pv;
	aubio_pickpeak_t			*parms;
		
	fvec_t						*vec;
	fvec_t						*onset;
	cvec_t						*fftgrain;//complex vector of buffersize, 1
	
	bool aubioOnsetFound;
	int buffersize, hopsize, pos;
	float threshold,  threshold2;

	//Raw value and Paul's "post-processed" version of the onset detection signal
	float rawDetectionValue, peakPickedDetectionValue;
	float maximumObservedValue;
};

#endif