/* ========================================
 *
 * Copyright J P LARCOM, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF J P LARCOM.
 *
 * ========================================
*/
#include "project.h"
#include "buffer.h"
#include "FFT_Q.h"
#include "blackman.h"
#include "cytypes.h"
#include "math.h"
#include "stdio.h"
#include "display.h"
#include "led_object.h"
#include "led_thesholds.h"

// constants
// the scale factors are based on the A-scale weighting scheme
// common in audio. the values are calculated as per the iec 61672-1, 
// then are modified to give an appealing visual effect.
// The values are computed in the matlab script "aScaleWeighting.m".
#define SCALE1          1156u
#define SCALE2          1734u
#define SCALE3          2601u * 0.85
#define SCALE4          3901u * 0.8
#define SCALE5          4996u * 0.7
#define SCALE6          6854u * 0.65
#define SCALE7          8954u * 0.65
#define SCALE8          11098u * 0.65
#define SCALE9          12014u * 0.65
#define SCALE10         13259u * 0.65
#define SCALE11         14944u
#define SCALE12         15525u
#define SCALE13         15040u
#define SCALE14         14331u
#define SCALE15         12404u
#define QVALUE          16384u
#define LEDLENGTH       135
#define LEDON           15
#define LEDOFF          0
#define COLHEIGHT       9
#define NUMOFBANDS      15

// debug flags



//===========external global variables==================//

// audio input buffer
volatile int16      audioBuff[2][BUFFLEN]   = { {0},{0} };
volatile uint8      buffInput               = 0;
volatile uint8      buffOutput              = 1;
volatile uint16     buffIndex               = 0; 
volatile uint8      buffFull                = 0;

// button press flag
volatile uint8      buttonPress             = 0;
volatile uint8      buttonDown              = 0;
volatile uint8      buttonUp                = 0;




// function prototypes
short signed int mean( short signed int [] );
short unsigned int stdDev( short signed int [], int );
void updateThresholds( uint8 direction, uint16 thrshldUP[], uint16 thrshldDOWN[]  );



int main(void)
{
    
    // led thresholds variable
    uint8 sensitivity_level = 0; 
    uint8 min_sensitivity = 0;
    uint8 max_sensitivity = 32;
    
    // A-Scale weights
    uint32 A_scale[15] = {SCALE1, SCALE2, SCALE3, SCALE4, SCALE5, SCALE6, SCALE7, SCALE8,
                          SCALE9, SCALE10, SCALE11, SCALE12, SCALE13, SCALE14, SCALE15 };
    
    
    //======== variables ================//
    short int fftData[2][BUFFLEN];
    int16 fftIndex;
    char buttonState = 0;
    char uartBuffer[48] = {0};
    char freq_index = 0;
    int32 gain = 16384;
    char led_index;
    uint32 temp = 0;
    char buttonUp = 1;
    char buttonDown = 1;
    unsigned int std[NUMOFBANDS]= {0};
    char index_threshold;
    
    // FFT freq resolution ~ 20Hz 41666/2048
    // see the matlab file logrithmicFreqBands.mlx to see explaination of the freq band start stop and widths 

    short int freqBand_LL[15] = {2,5,8,11,14,18,25,35,50,71,100,141,200,283,400};
    short int freqBand_UL[15] = {4,7,10,13,17,24,34,49,70,99,140,199,282,399,1024};
    
    
    // instantiate led objects which is an array of structs.
    led led_objects[LEDLENGTH];

    /*
    for(int index = 0*COLHEIGHT; index < 3*COLHEIGHT; index++)
    {
        led_init(&led_objects[index], LEDON, 14, 14, 180);      
    }
    
    for(int index = 3*COLHEIGHT; index < 6*COLHEIGHT; index++)
    {                                       
        led_init(&led_objects[index], LEDON, 145, 4, 60);      
    }
    
    for(int index = 6*COLHEIGHT; index < 9*COLHEIGHT; index++)
    {                                      
        led_init(&led_objects[index], LEDON, 167, 128, 76);      
    }
    
    for(int index = 9*COLHEIGHT; index < 12*COLHEIGHT; index++)
    {                                      
        led_init(&led_objects[index], LEDON, 64, 160, 216);      
    }
    
    for(int index = 12*COLHEIGHT; index < 15*COLHEIGHT; index++)
    {
        led_init(&led_objects[index], LEDON, 255, 255, 254);      
    }
    */
    
    for(int index = 0; index < LEDLENGTH; index = index + COLHEIGHT)
    {
        led_init(&led_objects[index], LEDON, 145, 4, 60);      
    }
    for(int index = 1; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                       //14, 14, 180
        led_init(&led_objects[index], LEDON, 185, 16, 50);      
    }
    
    
    for(int index = 2; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                       
        led_init(&led_objects[index], LEDON, 255, 30, 35);      
    }
    for(int index = 3; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                       
        led_init(&led_objects[index], LEDON, 210, 90, 50);      
    }
    
    for(int index = 4; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                      
        led_init(&led_objects[index], LEDON, 167, 150, 76);      
    }
    for(int index = 5; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                      
        led_init(&led_objects[index], LEDON, 130, 175, 76);      
    }
    
    for(int index = 6; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                      
        led_init(&led_objects[index], LEDON, 167, 150, 76);      
    }
    for(int index = 7; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                      
        led_init(&led_objects[index], LEDON, 210, 90, 50);      
    }
    
    for(int index = 8; index < LEDLENGTH; index = index + COLHEIGHT)
    {                                       //10, 30, 220
        led_init(&led_objects[index], LEDON, 255, 30, 35);      
    }
    for(int index = 9; index < LEDLENGTH; index = index + COLHEIGHT)
    {
        led_init(&led_objects[index], LEDON, 185, 16, 50);      
    }
    
    // create led_objects buffer
    uint8 display_buffer[LEDLENGTH * 4 + 8];
    update_buffer(display_buffer, led_objects, LEDLENGTH);
    
    //======================================================
    //  initialize system
    //======================================================
    CyDelay(500);
    PGA_1_Start();
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    UART_1_Start();
    SPI_led_Start();
    Pin_LE_Write( 0 );
    Pin_OE_Write( 1 );
    Pin_debug1_Write( 0 );
    isr_Pin_buttUp_Start();
    isr_Pin_buttDown_Start();
    isr_adc_eoc_Start();
    
    //DVDAC_1_Start();
            
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_PutString("COM port Open\r\n");
    
    // turn on display
    char charIndex;
    for( charIndex = 0; charIndex < LEDLENGTH; charIndex++ )
    {
        SPI_led_PutArray(display_buffer + charIndex*4, 4);
    }
    SPI_led_PutArray(display_buffer + charIndex*4, 8);
    
    while( 1 )
    {        
      
        if( buffFull )
        {
            Pin_debug1_Write( 1 );
            buffFull = 0;
 
            // Copy audio buffer to complex valued buffer and multiply with hann window
            
            for( fftIndex = 0; fftIndex < BUFFLEN; fftIndex++ )
            {
                fftData[REAL][fftIndex] = ( audioBuff[ buffOutput ][ fftIndex ] * blackmanWin[fftIndex] ) >> QFACTOR;
                fftData[IMAG][fftIndex] = 0;
            }
            
            // calculate FFT
            fft( fftData );
                   
            
            // calculate RMS of several frequency bands from freq spectrum and compare to thresholds
            // use hysteresis to avoid led's flickering

            
            for( freq_index = 0; freq_index < NUMOFBANDS; freq_index++)
            {
                // calculate std dev of the desired freq band
                std[freq_index] = fftRMS( fftData, freqBand_LL[freq_index], freqBand_UL[freq_index] );
                /* do not normalize for now
                // normalize the higer freq bands as they contain more freq bins and thus more noise / audio.
                if(index > 3)
                {
                    std[index] = std[index] >> 1;
                }
                */

                temp = ((uint32)std[freq_index] * A_scale[freq_index]) >> QFACTOR;
                std[freq_index] = (unsigned int)temp;

                
                index_threshold = 0;
                for( led_index = freq_index*COLHEIGHT; led_index < freq_index*COLHEIGHT+COLHEIGHT; led_index++ )
                {
                    if( led_brightness_read(&led_objects[led_index]) > LEDOFF )
                    {   
                        // if led was ON during previous output, use lower threshold
                        if( std[freq_index] > thrshldDOWN[freq_index][ index_threshold++ ] )
                        {
                            // if audio level is greater then threshold turn on led
                            led_brightness_write(&led_objects[led_index], LEDON);
                        }     
                        else
                        {
                            led_brightness_write(&led_objects[led_index], LEDOFF);   
                        }
                    }
                    else
                    {
                        // if led was OFF during previous output, use upper threshold
                        if( std[freq_index] > thrshldUP[freq_index][ index_threshold++ ] )
                        {
                            // if audio level is greater then threshold turn on led
                            led_brightness_write(&led_objects[led_index], LEDON);
                        } 
                        else
                        {
                            led_brightness_write(&led_objects[led_index], LEDOFF);   
                        }
                    }
                    
                }                   
            }// end for loop
            
            

            // Update Display
            update_buffer(display_buffer, led_objects, LEDLENGTH);
            char charIndex;
            for( charIndex = 0; charIndex < LEDLENGTH; charIndex++ )
            {
                SPI_led_PutArray(display_buffer + charIndex*4, 4);
            }
            SPI_led_PutArray(display_buffer + charIndex*4, 8);
             
            // send debug info to usb
            /*
            sprintf( uartBuffer, "Threshold values: %d, %d, %d, %d, %d, %d, %d, %d\r\n", 
                     thrshldUP[sensitivity_level][0], 
                     thrshldUP[sensitivity_level][1], 
                     thrshldUP[sensitivity_level][2], 
                     thrshldUP[sensitivity_level][3], 
                     thrshldUP[sensitivity_level][4], 
                     thrshldUP[sensitivity_level][5], 
                     thrshldUP[sensitivity_level][6],
                     thrshldUP[sensitivity_level][7]);
            */
            /*
            sprintf( uartBuffer, "f0, %d, f1, %d, f2, %d, f3, %d, f4, %d, f5, %d, f6, %d, f7, %d, f8, %d, f9, %d, f10, %d, f11, %d, f12, %d, f13, %d, f14 %d\r\n",
                     std[0], std[1], std[2], std[3], std[4], std[5], std[6], std[7], std[8], std[9], std[10], std[11], std[12], std[13], std[14] );
            UART_1_PutString(uartBuffer);            
            */
            /*
            sprintf( uartBuffer, "f0, %d, f1, %d, f2, %d, f3, %d, f4, %d, f5, %d\r\n",
            std[0], std[1], std[2], std[3], std[4], std[5] );
            UART_1_PutString(uartBuffer);
            */
            sprintf( uartBuffer, "%d, \r", std[0] );
            UART_1_PutString(uartBuffer);
            
            
            Pin_debug1_Write( 0 );
            
        }// end if( buffFull )
       
    }// end while( 1 )
}// end main()


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

void updateThresholds( uint8 direction, uint16 thrshldUP[], uint16 thrshldDOWN[] )
{
    if( direction == 0 )
    {
        // decrease thresholds
        
    }
    else if( direction == 1 )
    {
        // increase thresholds
        
    }
    
}



/* [] END OF FILE */
