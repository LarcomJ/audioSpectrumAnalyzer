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
#include "cytypes.h"
#include "math.h"
#include "stdio.h"



// debug flags
#define DACOUT
#define FILT1
//#define FILT2
//#define FILT3
//#define FILT4
//#define FILT5
//#define FILT6
//#define FILT7

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
    short int fftData[2][BUFFLEN];
    int16 fftIndex;
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
    
    
    // create buffer to store output of filter
    //short signed int filterOut[BUFFLEN] = {0};
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
            
            // calculate spectrum via FFT
            Pin_debug1_Write( 1 );
            for( fftIndex = 0; fftIndex < BUFFLEN; fftIndex++ )
            {
                fftData[REAL][fftIndex] = audioBuff[ buffOutput ][ fftIndex ];
            }
            
            fft( fftData );
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
