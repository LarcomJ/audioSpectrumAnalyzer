//////////////////////////////////////////////////////////////////////////////
//
//	FFT.c
//
//	Author: Jonathan P Larcom
//	Date created :	7 Apr 2019	
//
//	This library was created to compute the FFT for an audio signal analyzer.
//	The FFT length is fixed which reduces complexity and increases speed at the
//	cost of flexibility. Implimentation is Radix 2 Decimatin In Time and uses
//	fixed point arithmetic and is intended for 32 bit uControllers.
//
//
//  complex numbers are stored in 2d array with the following convention:
//	rows -- real or imag	& 	cols -- sample num
//
//	i.e.
//	a[real][0]	
//	a[imag][0]
//
//
//	Each stage the input is divided by 2 to compensate for the FFT gain of N.
//	this method of scaling gives better SNR performance compared to scaling 
//	the input by N before performing he FFT
//
///////////////////////////////////////////////////////////////////////////////


#include "FFT_Q.h"
#include "twiddleFactors.h"
#include "cytypes.h"
#include "math.h"


extern short int twiddleFactors[2][FFTLEN];


/////////// FUNCTION DEFENITIONS /////////////////////////

void fftMagnitude( short int fftResults[2][FFTLEN], short int fftMag[FFTLEN] )
{
	int temp = 0;
	short int index;
	
	for( index = 0; index < FFTLEN; index++ )
	{
		temp = ( ( fftResults[REAL][index] * fftResults[REAL][index] ) + ( fftResults[IMAG][index] * fftResults[IMAG][index] ) );
		fftMag[index] = (short int)sqrt(temp);
	}
	
}


/////////////////////////////////////////////////////
//
//	fftRMS() calculates the signal
//	RMS value from the fourier spectrum.
//	Note that the RMS is essentially the 
//	square root of the 1/N scaled signal
//	energy and parsevals theorem proves 
//	that the energy in the spectrum equals
//	the energy in the time domain.
//
/////////////////////////////////////////////////////
short int fftRMS( short int data[2][FFTLEN], short int startBin, short int endBin )
{
	int temp = 0;
	short int index;
	short int length = endBin - startBin;
	
	for( index = startBin; index <= endBin; index++ )
	{
		temp = temp + ( ( data[REAL][index] * data[REAL][index] ) + ( data[IMAG][index] * data[IMAG][index] ) );
	}
	
	return (short int)sqrt( 2 * temp  );
}

short int log2_Q( short int x )
{
	short int count;
	for( count = 0; count < 255; count++ )
	{
		x = x >> 1;
		if( x < 1 ){ break; }
	}
	
	return count;
}


short int bitReverseIndex( short int indexNum )
{
	char numOfBits = log2( FFTLEN );
	short int index;
	// push bits onto stack
	short int stack = 0;
	short int val;
	for( index = 0; index < numOfBits; index++ )
	{
		val = (indexNum&(1<<index)) << (numOfBits-index);
		if( val > 0 )
		{
			stack = stack | ( 1 << (numOfBits - 1 - index) );
		}
	}
	
	return stack;
}

void dft_2Pt( int x[], int y[] )
{
	y[0] = (x[0] + x[1]) >> 1;
	y[1] = (x[0] - x[1]) >> 1;
}


void fft_butterFlyStage( int input[2][2], int output[2][2], short int twiddleFac[2][2] )
{
	////////////////////////////////////////////////////////////////////////////////
	// twiddle factors are complex values therefor they require complex multiply
	// (aReal + jaImag)(bReal + jbImag) = aReal * bReal + j(aReal * bImag + aImag * bReal) - aImag * bImag = (aReal * bReal - aImag * bImag) + j(aReal * bImag + aImag * bReal)
	//
	//	aReal = input[0][0] OR input[0][1]
	//	aImag = input[1][0] OR input[1][1]
	//	bReal = twiddleFac[0][0] OR twiddleFac[0][1]
	//	bImag = twiddleFac[1][0] OR twiddleFac[1][1]
	//
	//
	//	y0 = x0 + W0 * x1
	//  y1 = x0 + W1 * x1
	//
	////////////////////////////////////////////////////////////////////////////////
	int aReal = input[REAL][0];
	int aImag = input[IMAG][0];
	int bReal = input[REAL][1];
	int bImag = input[IMAG][1];
	int wReal = twiddleFac[REAL][0];
	int wImag = twiddleFac[IMAG][0];
	
	output[REAL][0] = aReal + ( ( ( bReal * wReal ) - ( bImag * wImag ) ) >> QFACTOR );
	output[IMAG][0] = aImag + ( ( ( bReal * wImag ) + ( bImag * wReal ) ) >> QFACTOR );
	
	wReal = twiddleFac[REAL][1];
	wImag = twiddleFac[IMAG][1];
		
		
	output[REAL][1] = aReal + ( ( ( bReal * wReal ) - ( bImag * wImag ) ) >> QFACTOR );
	output[IMAG][1] = aImag + ( ( ( bReal * wImag ) + ( bImag * wReal ) ) >> QFACTOR );
}


void fft( short int data[2][FFTLEN] )
{
	short int index;
	short int stageNum;
	short int numOfStages = log2( FFTLEN );
	short int groupNum;
	short int groupLen;
	short int indexOffset;
	short int numOfGroups;
	short int groupOffset;
	int tempOutput[2][2] = { {0},{0} };
	int tempInput[2][2] = { {0},{0} };
	int dftResult[2][FFTLEN] = { {0},{0} };
	short int tempTwiddle[2][2] = { {0},{0} };
	short int tempTwiddle_vector[2][FFTLEN] = { {0}, {0} };
	short int twiddleOffset;
	short int twiddleVecInd;
	short int twiddleIndex;
	short int bitReverseInd[2];
	int input[2];
	int output[2];
	
	// compute 2 point dft for all data in input vector
	
	for( index = 0; index < FFTLEN; index = index + 2 )
	{
		bitReverseInd[0] = bitReverseIndex( index );
		bitReverseInd[1] = bitReverseIndex( index + 1 );
		input[0] = data[REAL][bitReverseInd[0]];
		input[1] = data[REAL][bitReverseInd[1]];
		
		dft_2Pt( input, output );
		
		dftResult[REAL][index] = output[0];
		dftResult[REAL][index+1] = output[1];
		
	}
	
	// for each stage combine data using grouping and butterflys
	for( stageNum = 2; stageNum <= numOfStages; stageNum++ )
	{
		
		groupLen = 1 << stageNum;
		numOfGroups = FFTLEN >> stageNum;
		indexOffset = 1 << (stageNum-1);
		twiddleOffset = FFTLEN >> stageNum;
		
		// extract twiddle factors for the group
		for( twiddleIndex = 0, twiddleVecInd = 0; twiddleIndex < FFTLEN; twiddleIndex = twiddleIndex + twiddleOffset, twiddleVecInd++ )
		{
			tempTwiddle_vector[REAL][twiddleVecInd] = twiddleFactors[REAL][twiddleIndex];
			tempTwiddle_vector[IMAG][twiddleVecInd] = twiddleFactors[IMAG][twiddleIndex];
		}
		
		// for each group in the stage
		for( groupNum = 0; groupNum < numOfGroups; groupNum++ )
		{	
			
			// for each butterfly in the group 
			groupOffset = groupLen * groupNum;
			for( index = 0; index < (groupLen>>1); index++ )
			{
				// extract data from array
				tempInput[REAL][0] = dftResult[REAL][ groupOffset + index ] >> 1;	// var1 real divide by 2 to compensate for FFT scaling 
				tempInput[IMAG][0] = dftResult[IMAG][ groupOffset + index ] >> 1;	// var1 imag divide by 2 to compensate for FFT scaling 
				tempInput[REAL][1] = dftResult[REAL][ groupOffset + index + indexOffset ] >> 1;	// var2 real divide by 2 to compensate for FFT scaling 
				tempInput[IMAG][1] = dftResult[IMAG][ groupOffset + index + indexOffset ] >> 1;	// var2 imag divide by 2 to compensate for FFT scaling 
				
				// extract twiddle factors
				tempTwiddle[REAL][0] = tempTwiddle_vector[REAL][ index ];
				tempTwiddle[IMAG][0] = tempTwiddle_vector[IMAG][ index ];
				tempTwiddle[REAL][1] = tempTwiddle_vector[REAL][ index + indexOffset ];
				tempTwiddle[IMAG][1] = tempTwiddle_vector[IMAG][ index + indexOffset ];
				
				
				// compute fft butterfly
				fft_butterFlyStage( tempInput, tempOutput, tempTwiddle );
				
				// save results to data array
				dftResult[REAL][ groupOffset + index ] 	= tempOutput[REAL][0];	// var1 real
				dftResult[IMAG][ groupOffset + index ] 	= tempOutput[IMAG][0];	// var1 imag
				dftResult[REAL][ groupOffset + index + indexOffset ] = tempOutput[REAL][1];	// var2 real
				dftResult[IMAG][ groupOffset + index + indexOffset ] = tempOutput[IMAG][1];	// var2 imag
							
				
			}
		}
	}
	
	for(index = 0; index < FFTLEN; index++) 
	{
		data[REAL][index] = dftResult[REAL][index];
		data[IMAG][index] = dftResult[IMAG][index];
	}
	
	
}





























