

#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#include "fftw3.h"

class Chromagram
{
public:		
	Chromagram();											// constructor
	~Chromagram();											// destructor
	void chromacalc(float frame[]);
	void initialise(int framesize,int hsize);
	void processframe(float frame[]);
		
	float rawChroma[12];
	float normalisedChroma[12];//that adds up to 1
	float chroma[12];
	float chroma_low[12];
	int chromaready;	
		
	float maximumChromaValue;
	int bsize;
	
private:
	void hamming(int N);
	void makeprofiles();
	void getmagspectrum();
	void generatechromagram();
	//float euclidean(float v1[],float v2[],int length);
	float max(float array[],int length);
	int minindex(float array[],int length);
	void downsampleframe(float frame[]);


	int count;
	int hopsize;
	int fsize;
	int calcrate;
	float ratio;
	
	float *buffer;											// audio buffer
	float *wbuffer;											// windowed audio buffer
	float *win;												// hanning window
	float *mag;
	float *d_frame;
	float *hop;	
	
	
	float note[12];
	int harmonics;
	int octaves;
	int search;
	
	fftwf_plan p;											// create fft plan
	fftwf_complex *out;										// to hold complex fft values
	
	float pi;
	

};

#endif
