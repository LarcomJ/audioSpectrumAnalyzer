#ifndef FFT
#define FFT
//////////////////////////////////////////////////////////
//
//	FFT.h
//
//
//////////////////////////////////////////////////////////
    
#include "cytypes.h"

#define FFTLEN  2048
#define QFACTOR 14
#define REAL	0
#define IMAG	1


void dft_2Pt( int x[], int y[] );
void fft_butterFlyStage( int input[2][2], int output[2][2], short int twiddleFac[2][2] );
void fft( short int data[2][FFTLEN] );
short int log2_Q( short int x );
short int bitReverseIndex( short int indexNum );
void fftMagnitude( short int fftResults[2][FFTLEN], short int fftMag[FFTLEN] );
short int fftRMS( short int data[2][FFTLEN], short int startBin, short int endBin );









#endif
