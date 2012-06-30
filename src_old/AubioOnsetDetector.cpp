/*
 *  AubioOnsetDetector.cpp
 *  aubioonset~
 *
 *  Created by Andrew Robertson on 13/08/2010.
 *  Adapted from code by Paul Brossier
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AubioOnsetDetector.h"


AubioOnsetDetector :: AubioOnsetDetector(){
	buffersize = 1024;
	hopsize = 512;
		//aubio related setup
		o = new_aubio_onsetdetection(aubio_onset_complex, buffersize, 1);//initially in complex mode
		pv = (aubio_pvoc_t *)new_aubio_pvoc(buffersize, hopsize, 1);
		parms = new_aubio_peakpicker(threshold);
		vec = (fvec_t *)new_fvec(hopsize,1);
		
		threshold = 1;
		threshold2 = -70.;
		rawDetectionValue = 0;
		peakPickedDetectionValue = 0;
		pos = 0;
		maximumObservedValue = 1;
	//printf("CREATING AUBIO ONSET :: SETTING pos to be %i \n", pos);
}

AubioOnsetDetector :: ~AubioOnsetDetector(){
	printf("AUBIO ONSET DETECTOR IS FREED\n");
//	del_aubio_onsetdetection(o);
	aubio_onsetdetection_free (o);

	del_aubio_pvoc(pv);
	del_aubio_peakpicker(parms);

	del_fvec(vec);
	del_fvec(onset);
	del_cvec(fftgrain);
	
		
		
}

void AubioOnsetDetector :: initialise(){
	//reinitialises our object
		o = new_aubio_onsetdetection(aubio_onset_complex, buffersize, 1);//initially in complex mode
		pv = (aubio_pvoc_t *)new_aubio_pvoc(buffersize, hopsize, 1);
		parms = new_aubio_peakpicker(threshold);
		vec = (fvec_t *)new_fvec(hopsize,1);
		pos = 0;
				fvec_write_sample(vec, 0.234, 0, pos);
				fftgrain  = (cvec_t *)new_cvec(buffersize,1);
				onset = (fvec_t *)new_fvec(1,1);
}

bool AubioOnsetDetector :: processframe(float frame[], int n){
	bool newFrameResult = false;

	//Paul Brossier's aubioonsetclass~ code ported from Pd example
		int j,isonset;//isonset is true when Aubio thinks there is an onset
		
	//we also output the raw detection function - useful for programming alternative detection functions
	//and for beat tracking etc
	
	for (j=0;j < n;j++) {
		// write input to datanew 
		
		fvec_write_sample(vec, frame[j], 0, pos);//vec->data[0][pos] = frame[j]
		//time for fft
	
			if (pos == hopsize-1) {  //hopsize is 512  

			newFrameResult = true;
			aubioOnsetFound = false;
			// block loop 
			aubio_pvoc_do (pv,vec, fftgrain);
			
			fftgrain->norm[0][0] = fabs(fftgrain->norm[0][0]);
			//AR: added hack to solve bug that norm[0][0] is negative sometimes. 
			
			aubio_onsetdetection(o, fftgrain, onset);
			rawDetectionValue = onset->data[0][0];
		
			if (rawDetectionValue > maximumObservedValue)
				maximumObservedValue = rawDetectionValue;
				
		//Paul Brossier's method to return value of peak picking process
	
	//	smpl_t my_sample_value;
		peakPickedDetectionValue = aubio_peakpick_pimrt_getval(parms); 
	//peakPickedDetectionValue = my_sample_value;
	//	outlet_float(x->detectionFunctionOutlet, my_sample_value);
	//	outlet_float(x->rawDetectionFunctionOutlet, x->onset->data[0][0]);
		
			isonset = aubio_peakpick_pimrt(onset,parms);
			if (isonset) {
				// test for silence 
				if (aubio_silence_detection(vec, threshold2)==1){
					isonset=0;
					}
				else{
					aubioOnsetFound = true;
					}
			}//end if (isonset)
	
			// end of block loop 
			pos = -1; // so it will be zero next j loop 
		}
		pos++;
		
//	outL[j] = frame[j];//would have added for Max object version so the signal is "see through": outputting the input signal
	
	}
//end of Paul's code

return newFrameResult;
		
}

float AubioOnsetDetector :: getRawDetectionFrame(){
return rawDetectionValue;	
}

float AubioOnsetDetector :: getPeakPickedDetectionFrame(){
return peakPickedDetectionValue;	
}


void AubioOnsetDetector :: onsetclass_energy(){
		//aubio_onsetdetection_type
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_energy, buffersize, 1);
}



void AubioOnsetDetector :: onsetclass_hfc(){
/** High Frequency Content onset detection function
 
  This method computes the High Frequency Content (HFC) of the input spectral
  frame. The resulting function is efficient at detecting percussive onsets.

  Paul Masri. Computer modeling of Sound for Transformation and Synthesis of
  Musical Signal. PhD dissertation, University of Bristol, UK, 1996.*/
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_hfc, buffersize, 1);
}


void AubioOnsetDetector :: onsetclass_complex(){
		//aubio_onsetdetection_type
		//Complex Domain Method onset detection function 
		//Christopher Duxbury, Mike E. Davies, and Mark B. Sandler. Complex domain
		//onset detection for musical signals. In Proceedings of the Digital Audio
		//Effects Conference, DAFx-03, pages 90-93, London, UK, 2003.
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_complex, buffersize, 1);
}

void AubioOnsetDetector :: onsetclass_phase(){
/** Phase Based Method onset detection function 

  Juan-Pablo Bello, Mike P. Davies, and Mark B. Sandler. Phase-based note onset
  detection for music signals. In Proceedings of the IEEE International
  Conference on Acoustics Speech and Signal Processing, pages 441¬≠444,
  Hong-Kong, 2003.*/
  		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_phase, buffersize, 1);
		
}

void AubioOnsetDetector :: onsetclass_specdiff(){
		/* Spectral difference method onset detection function 
		Jonhatan Foote and Shingo Uchihashi. The beat spectrum: a new approach to
		rhythm analysis. In IEEE International Conference on Multimedia and Expo
		(ICME 2001), pages 881¬≠884, Tokyo, Japan, August 2001.
		*/
		//aubio_onsetdetection_type
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_specdiff, buffersize, 1);
}

void AubioOnsetDetector :: onsetclass_kl(){
		/** Kullback-Liebler onset detection function 
  
			Stephen Hainsworth and Malcom Macleod. Onset detection in music audio
			signals. In Proceedings of the International Computer Music Conference
			(ICMC), Singapore, 2003.
		*/
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_kl, buffersize, 1);
}

void AubioOnsetDetector :: onsetclass_mkl(){

		/** Modified Kullback-Liebler onset detection function 

		Paul Brossier, ``Automatic annotation of musical audio for interactive
		systems'', Chapter 2, Temporal segmentation, PhD thesis, Centre for Digital
		music, Queen Mary University of London, London, UK, 2003.*/		
		aubio_onsetdetection_free (o);
		o = new_aubio_onsetdetection(aubio_onset_hfc, buffersize, 1);
}

