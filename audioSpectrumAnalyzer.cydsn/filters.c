/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "filters.h"

//==============================================================================
//	IIR Biquad filter
//
//	nth order filter that impliments a direct form biquad filter. The function
//  is recursive allowing for N 2nd order stages to be calculated in cascade.
//
//	sample_t -- struct pointer to input sample buffer
//	output_t -- struct pointer to output buffer
//	coeff	 -- this holds the filter coefficients and must follow the form below
//	order	 -- order of the filter, must be multiples of 2
//
//										   |   1st biquad   | |   2nd biquad   | .......
//	coeff is an linear array of the form [ b0 b1 b2 a0 a1 a2  b3 b4 b5 a3 a4 a5 ....... ]
//											
//	
// =============================================================================
short signed int iir_biquad_4th_order( short signed int sample, reg_struct *reg, short signed int coeff[] )
{
	signed int acc = 0;
	
    //================= First BiQuad stage =======================//
    
	// scale input to avoid overflow
	sample = ( short signed int )( ( ( signed int )sample * ( signed int )reg->input_scale[ 0 ] ) >> Q_format );
	
	// compute direct form biquad filter
	acc = ( (signed int)sample * (signed int)coeff[ 0 ] );
	acc = acc + ( (signed int)reg->x1[ 0 ] * (signed int)coeff[ 1 ] );
	acc = acc + ( (signed int)reg->x2[ 0 ] * (signed int)coeff[ 2 ] );
	acc = acc - ( (signed int)reg->y1[ 0 ] * (signed int)coeff[ 4 ] );
	acc = acc - ( (signed int)reg->y2[ 0 ] * (signed int)coeff[ 5 ] );
	acc = acc >> Q_format;
	
	
	// saturate the output if overflow occured
	if( acc > overflow ) 
	{ acc = overflow; }
	else if( acc < underflow )
	{ acc = underflow; }
	
	
	// shift data in registers
	reg->x2[ 0 ] = reg->x1[ 0 ];
	reg->x1[ 0 ] = sample;
	reg->y2[ 0 ] = reg->y1[ 0 ];
	reg->y1[ 0 ] = ( short signed int )acc;
    
    //================= Second BiQuad stage =======================//
    
	// scale input to avoid overflow
	sample = ( short signed int )( ( ( signed int )acc * ( signed int )reg->input_scale[ 1 ] ) >> Q_format );
	
	// compute direct form biquad filter
	acc = ( (signed int)sample * (signed int)coeff[ 6 ] );
	acc = acc + ( (signed int)reg->x1[ 1 ] * (signed int)coeff[ 7 ] );
	acc = acc + ( (signed int)reg->x2[ 1 ] * (signed int)coeff[ 8 ] );
	acc = acc - ( (signed int)reg->y1[ 1 ] * (signed int)coeff[ 10 ] );
	acc = acc - ( (signed int)reg->y2[ 1 ] * (signed int)coeff[ 11 ] );
	acc = acc >> Q_format;
	
	
	// saturate the output if overflow occured
	if( acc > overflow ) 
	{ acc = overflow; }
	else if( acc < underflow )
	{ acc = underflow; }
    
    // shift data in registers
	reg->x2[ 1 ] = reg->x1[ 1 ];
	reg->x1[ 1 ] = sample;
	reg->y2[ 1 ] = reg->y1[ 1 ];
	reg->y1[ 1 ] = ( short signed int )acc;
	
	// scale output
	//acc = ( acc * ( signed int )reg->gain[ stage ] ) >> Q_format;
	//acc = ( acc * reg->output_scale[ stage ] ) >> Q_format;
		
	return ( short signed int )acc;
	
}

//==========================================================================
//
//	iir coupled form filter by Gold and Rader
//
//
//
//
//
//
//==========================================================================
short signed int iir_coupled_form( short signed int sample, reg_struct *reg, short signed int coeff[] )
{
	signed int acc = 0;
	signed int acc1 = 0;
	signed int acc2 = 0;
	
    //=================== First 2nd order stage ==================//
	
	// scale input to avoid overflow
	//sample = ( short signed int )( ( ( signed int )sample * ( signed int )reg->input_scale[ 0 ] ) >> Q_format );
	sample = sample >> 1;
    
	// compute feed forward branch
	acc = ( (signed int)sample * (signed int)coeff[ 0 ] );
	acc = acc + ( (signed int)reg->x1[ 0 ] * (signed int)coeff[ 1 ] );
	acc = acc + ( (signed int)reg->x2[ 0 ] * (signed int)coeff[ 2 ] );
	acc = acc >> Q_format;
	
	
	// compute feed back branch
	acc1 = acc + ( (signed int)reg->y1[ 0 ] * (signed int)coeff[ 3 ] >> Q_format ) - ( (signed int)reg->y2[ 0 ] * (signed int)coeff[ 4 ] >> Q_format );
	acc2 = ( (signed int)reg->y1[ 0 ] * (signed int)coeff[ 4 ] >> Q_format ) + ( (signed int)reg->y2[ 0 ] * (signed int)coeff[ 3 ] >> Q_format );
	
	
	// saturate the output if overflow occured
	if( acc2 > overflow ) 
	{ acc2 = overflow; }
	else if( acc2 < underflow )
	{ acc2 = underflow; }
	
	// shift data in registers
	reg->x2[ 0 ] = reg->x1[ 0 ];
	reg->x1[ 0 ] = sample;
	reg->y1[ 0 ] = ( short signed int )acc1;
	reg->y2[ 0 ] = ( short signed int )acc2;
    
    //=================== Second 2nd order stage ==================//
    
    // scale input to avoid overflow
	//sample = ( short signed int )( ( ( signed int )acc2 * ( signed int )reg->input_scale[ 1 ] ) >> Q_format );
	sample = acc2;
	
	// compute feed forward branch
	acc = ( (signed int)sample * (signed int)coeff[ 5 ] );
	acc = acc + ( (signed int)reg->x1[ 1 ] * (signed int)coeff[ 6 ] );
	acc = acc + ( (signed int)reg->x2[ 1 ] * (signed int)coeff[ 7 ] );
	acc = acc >> Q_format;
	
	
	// compute feed back branch
	acc1 = acc + ( (signed int)reg->y1[ 1 ] * (signed int)coeff[ 8 ] >> Q_format ) - ( (signed int)reg->y2[ 1 ] * (signed int)coeff[ 9 ] >> Q_format );
	acc2 = ( (signed int)reg->y1[ 1 ] * (signed int)coeff[ 9 ] >> Q_format ) + ( (signed int)reg->y2[ 1 ] * (signed int)coeff[ 8 ] >> Q_format );
	
	
	// saturate the output if overflow occured
	if( acc2 > overflow ) 
	{ acc2 = overflow; }
	else if( acc2 < underflow )
	{ acc2 = underflow; }
	
	// shift data in registers
	reg->x2[ 1 ] = reg->x1[ 1 ];
	reg->x1[ 1 ] = sample;
	reg->y1[ 1 ] = ( short signed int )acc1;
	reg->y2[ 1 ] = ( short signed int )acc2;
	
		
	return ( short signed int )acc2;
	
}


/* [] END OF FILE */
