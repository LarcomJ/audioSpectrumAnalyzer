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

#ifndef FILTERS
    #define FILTERS
    


    #define Q_format	14
    #define overflow	0x7fff
    #define underflow	-0x8000
    
    // filter struct defenition

    typedef struct
    {
    	// first layer register for the feed forward path, each element represents
    	// the nth cascade stage
    	short signed int *x1;
    	// second layer register for the feed forward path
    	short signed int *x2;
    	// first layer register for the feed back path
    	short signed int *y1;
    	// second layer register for the feed back path
    	short signed int *y2;
    	// input signal scaling factor due to stage gain
    	short signed int *input_scale;

    	
    } reg_struct;
    

    
    //************ function prototypes ***********************//
    short signed int iir_biquad_4th_order( short signed int sample, reg_struct *reg, short signed int coeff[] );
    short signed int iir_coupled_form( short signed int sample, reg_struct *reg, short signed int coeff[] );
    
    
    
#endif

/* [] END OF FILE */
