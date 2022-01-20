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
#include "project.h"
#include "buffer.h"
#include "FFT_Q.h"
#include "filters.h"
#include "cytypes.h"
#include "math.h"
#include "stdio.h"



// debug flags
#define DACOUT
#define FILT1
#define FILT2
#define FILT3
#define FILT4
#define FILT5
#define FILT6
#define FILT7

//===========external global variables==================//

// audio input buffer
volatile int16      audioBuff[2][BUFFLEN] = { {0},{0} };
volatile uint8      buffInput = 0;
volatile uint8      buffOutput = 1;
volatile uint16     buffIndex = 0; 
volatile uint8      buffFull = 0;

// led thresholds = int16( 0.707 * floor((0.8*2^14-1)/9) * [1:8] )
uint16 thrshld[8] = {1029, 2059, 3088, 4118, 5147, 6176, 7206, 8235 };


// function prototypes
short signed int mean( short signed int [] );
short unsigned int stdDev( short signed int [], int );



int main(void)
{
    //======== variables ================//
    //short int fftData[2][BUFFLEN];
    //int16 fftIndex;
    char buttonState = 0;
    char uartBuffer[48] = {0};
    char index = 0;
    char index2 = 0;
    char sensitivityButtState = 0;
    char sensitivityState = 0;
    char buttonUp = 1;
    char buttonDown = 1;
    
    //======================================================
    //  initialize system
    //======================================================
    CyDelay(500);
    PGA_1_Start();
    isr_adc_eoc_Start();
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    UART_1_Start();
    SPI_led_Start();
    Pin_LE_Write( 0 );
    Pin_debug1_Write( 0 );
    
    //DVDAC_1_Start();
    
    // initialize filters
    short signed int x1_filt1[2] = {0};
	short signed int x2_filt1[2] = {0};
	short signed int y1_filt1[2] = {0};
	short signed int y2_filt1[2] = {0};
	short signed int input_scaling_filt1[ 2 ] = { 128, 128 };
	reg_struct temp_reg_filt1 = { x1_filt1, x2_filt1, y1_filt1, y2_filt1, input_scaling_filt1 };
	
	// create filter coeff
    short signed int coeff_filt1[ 12 ] = { 64, 128, 64, 16288, 227, 
                                           64, 128, 64, 16157, 93    }; // signed fixed point coeff Q14
    
    short signed int x1_filt2[2] = {0};
	short signed int x2_filt2[2] = {0};
	short signed int y1_filt2[2] = {0};
	short signed int y2_filt2[2] = {0};
	short signed int input_scaling_filt2[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt2 = { x1_filt2, x2_filt2, y1_filt2, y2_filt2, input_scaling_filt2 };
	
	// create filter coeff
    short signed int coeff_filt2[ 12 ] = { 6861, 0, -6861, 16144, 863, 
										   6861, 0, -6861, 16245, 521   }; // signed fixed point coeff Q14
    
    short signed int x1_filt3[2] = {0};
	short signed int x2_filt3[2] = {0};
	short signed int y1_filt3[2] = {0};
	short signed int y2_filt3[2] = {0};
	short signed int input_scaling_filt3[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt3 = { x1_filt3, x2_filt3, y1_filt3, y2_filt3, input_scaling_filt3 };
	
	// create filter coeff
    short signed int coeff_filt3[ 12 ] = { 5792, 0, -5792, 15847, 1953, 
										   5792, 0, -5792, 16059, 1286   }; // signed fixed point coeff Q14
    
    short signed int x1_filt4[2] = {0};
	short signed int x2_filt4[2] = {0};
	short signed int y1_filt4[2] = {0};
	short signed int y2_filt4[2] = {0};
	short signed int input_scaling_filt4[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt4 = { x1_filt4, x2_filt4, y1_filt4, y2_filt4, input_scaling_filt4 };
	
	// create filter coeff
    short signed int coeff_filt4[ 12 ] = { 5734, 0, -5734, 15095, 3781, 
										   5734, 0, -5734, 15638, 2522   }; // signed fixed point coeff Q14
    
    short signed int x1_filt5[2] = {0};
	short signed int x2_filt5[2] = {0};
	short signed int y1_filt5[2] = {0};
	short signed int y2_filt5[2] = {0};
	short signed int input_scaling_filt5[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt5 = { x1_filt5, x2_filt5, y1_filt5, y2_filt5, input_scaling_filt5 };
	
	// create filter coeff
    short signed int coeff_filt5[ 12 ] = { 6083, 0, -6083, 13132, 6779, 
										   6083, 0, -6083, 14612, 4589   }; // signed fixed point coeff Q14
    
    short signed int x1_filt6[2] = {0};
	short signed int x2_filt6[2] = {0};
	short signed int y1_filt6[2] = {0};
	short signed int y2_filt6[2] = {0};
	short signed int input_scaling_filt6[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt6 = { x1_filt6, x2_filt6, y1_filt6, y2_filt6, input_scaling_filt6 };
	
	// create filter coeff
    short signed int coeff_filt6[ 12 ] = { 7209, 0, -7209, 11947, 7800, 
										   7209, 0, -7209, 7966, 10752   }; // signed fixed point coeff Q14
    
    short signed int x1_filt7[2] = {0};
	short signed int x2_filt7[2] = {0};
	short signed int y1_filt7[2] = {0};
	short signed int y2_filt7[2] = {0};
	short signed int input_scaling_filt7[ 2 ] = { 160, 160 };
	reg_struct temp_reg_filt7 = { x1_filt7, x2_filt7, y1_filt7, y2_filt7, input_scaling_filt7 };
	
	// create filter coeff
    short signed int coeff_filt7[ 12 ] = { 8920, 0, -8920, 5152, 10656, 
										   8920, 0, -8920, -3952, 10980   }; // signed fixed point coeff Q14
    
    
    
    // create buffer to store output of filter
    short signed int filterOut[BUFFLEN] = {0};
    unsigned int filterOutIndex = 0;
    char ledsOut[ 7 ] = {0};
    char ledVal = 0;
    signed int avg = 0;
    signed int std = 0;
        
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_PutString("COM port Open\r\n");
    
    while( 1 )
    {
        if( !Pin_Button_Read() )
        {
            CyDelay(2000);
            if( !Pin_Button_Read() )
            {
                buttonState ^= 1;   
                Pin_Led_Write( ~Pin_Led_Read() );
            }
            buffFull = 0;
        }
        
        if( buffFull )
        {
            buffFull = 0;
            
            buttonUp = Pin_buttUp_Read();
            buttonDown = Pin_buttDown_Read();
            
            // adjust volume sensitivity by scaling the thresholds when user presses up button or down button
            if( (buttonUp == 0 ) || ( buttonDown == 0 ) )
            {
                if( sensitivityButtState == 0 )
                {
                    sensitivityButtState = 1;    
                }   
                else
                {
                    sensitivityButtState = 0;
                    if( buttonUp == 0 )
                    {
                        if( sensitivityState < 10 )
                        {
                            sensitivityState = sensitivityState + 1;       
                            for( index2 = 0; index2 < 8; index2++ )
                            {
                                // 0.8 * 2^14 = 13107
                                thrshld[index2] = (uint16)( ( (unsigned int)thrshld[index2] * 13107 ) >> 14 );
                            }
                            
                        }
                    }
                    if( buttonDown == 0 )
                    {
                        if( sensitivityState > 0 )
                        {
                            sensitivityState = sensitivityState - 1;    
                            for( index2 = 0; index2 < 8; index2++ )
                            {
                                // 1.2 * 2^14 = 19661
                                thrshld[index2] = (uint16)( ( (unsigned int)thrshld[index2] * 19661 ) >> 14 );
                            }
                        }   
                    }
                }
            }
            
            // filter data
            
            #ifdef FILT1
            Pin_debug1_Write( 1 );
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt1, coeff_filt1 );                 
            }
            
            avg = mean( filterOut );
            
            std = stdDev( filterOut , avg );
            
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[0] = ledVal;
            
            #endif
            
            #ifdef FILT2
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt2, coeff_filt2 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[1] = ledVal;
            #endif
            
            #ifdef FILT3
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt3, coeff_filt3 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[2] = ledVal;
            #endif
            
            #ifdef FILT4
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt4, coeff_filt4 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[3] = ledVal;
            #endif
            
            #ifdef FILT5
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt5, coeff_filt5 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[4] = ledVal;
            #endif
            
            #ifdef FILT6
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt6, coeff_filt6 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[5] = ledVal;
            #endif
            
            #ifdef FILT7
            for( filterOutIndex = 0; filterOutIndex < BUFFLEN; filterOutIndex++ )
            {
                filterOut[ filterOutIndex ] = iir_coupled_form( audioBuff[buffOutput][filterOutIndex], &temp_reg_filt7, coeff_filt7 );                 
            }
            avg = mean( filterOut );
            std = stdDev( filterOut , avg );
            for( index = 0; index < 8; index++ )
            {
                if( std > thrshld[ index ] )
                {
                    ledVal = ledVal | ( 1 << index );
                }
                
            }
            ledsOut[6] = ledVal;
            #endif
            
            Pin_debug1_Write( 0 );

            
            if( buttonState )
            {
                //ledVal = 0;
                Pin_LE_Write( 1 );
                SPI_led_WriteByte( ledsOut[0] );
                CyDelayUs( 20 );
                Pin_LE_Write( 0 );
                sprintf( uartBuffer, "Mean: %d \r\nStd Dev: %d \r\nLed Val: %d%d%d%d%d%d%d%d \r\nSensitivity: %d\r\n", avg, std, (ledVal>>7)&0x01, (ledVal>>6)&0x01, (ledVal>>5)&0x01, (ledVal>>4)&0x01, (ledVal>>3)&0x01, (ledVal>>2)&0x01, (ledVal>>1)&0x01, (ledVal>>0)&0x01, sensitivityState );
                //UART_1_PutString(uartBuffer);
                ledVal = 0;
            }
            else
            {
                Pin_LE_Write( 1 );
                SPI_led_WriteByte( 0x00 );
                CyDelayUs( 20 );
                Pin_LE_Write( 0 );   
            }


        }// end if( buffFull )
       
    }
}


short signed int mean( short signed int data[] )
{
    short int index;
    int acc = 0;
    for( index = 0; index < BUFFLEN; index++ )
    {
        acc = acc + data[index];
    }
    return (acc >> LOG2_BUFFLEN);
}

short unsigned int stdDev( short signed int data[], int avg )
{
    short int index;
    unsigned int acc = 0;
    for( index = 0; index < BUFFLEN; index++ )
    {
        acc = acc + ( ( (data[index] - avg) * (data[index] - avg) ) >> LOG2_BUFFLEN );
    }
    
    return (short unsigned int)sqrt( acc );
}



/* [] END OF FILE */
