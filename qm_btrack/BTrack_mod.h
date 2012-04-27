/*
 *  BTrack.h
 *  BTrack
 *
 *  Created by Adam Stark on 15/06/2009.
 *  Copyright 2009 Centre for Digital Music, Queen Mary University of London. All rights reserved.
 *
 */


//TO DO

//Investigate precision

//add in beat times 

#ifndef __BTRACK_H
#define __BTRACK_H

//#include "fftw3.h"

class BTrack {
	
public:
	BTrack();				// constructor
	~BTrack();				// destructor	
	
	void initialise(int fsize);
	void process(float df_sample);
	void plotdfbuffer();
	void updatecumscore(float df_sample);
	void predictbeat();
	void dfconvert();
	void calcTempo();
	void adapt_thresh(float x[],int N);
	float mean_array(float array[],int start,int end);
	void normalise(float array[],int N);
	void acf_bal(float df_thresh[]);
	void getrcfoutput();
	void settempo(float tempo);
	void fixtempo(float tempo);
	void unfixtempo();
	void setparams(float a,float t);
	void reset();
	void setTempoByTapping(long& ms);
	
	int playbeat;
	float cscoreval;

	float bperiod;
	float tempo;
	
	double anrBeatPeriodEstimate;//added by Andrew Robertson
	//this aims to be a more accurate slow moving estimate
	//not shifting between discrete values

	float *cumscore;			// to hold cumulative score
	float maxCumScore;
	
private:
	
	// buffers
	float *dfbuffer;			// to hold detection function
	float df512[512];			// to hold resampled detection function 

	
	float acf[512];				// to hold autocorrelation function
	
	float wv[128];				// to hold weighting vector
	
	float rcf[128];				// to hold comb filter output
	float t_obs[41];			// to hold tempo version of comb filter output
	
	float delta[41];			// to hold final tempo candidate array
	float prev_delta[41];		// previous delta
	float prev_delta_fix[41];	// fixed tempo version of previous delta
	
	float t_tmat[41][41];		// transition matrix
	
	
	// parameters
	float tightness;
	float alpha;

	
	
	float p_fact;
	
	
	//
	int m0;				// indicates when the next point to predict the next beat is
	int beat;
	
	int dfbuffer_size;
		
	
	int framesize;

	long lastTapTime;
	

	
	int tempofix;
};

#endif
