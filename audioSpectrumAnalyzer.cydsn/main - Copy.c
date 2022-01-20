/* ========================================
 *
 * Copyright J P lARCOM, 2019
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
#include "hamming.h"
#include "cytypes.h"
#include "math.h"
#include "stdio.h"
#include "display.h"
#include "led_object.h"

// constants
// the scale factors are based on the A-scale weighting scheme
// common in audio. the values are calculated as x/2^16 where 
// x is defined to be dB = 10 log10(x) solve for x.
#define SCALE63HZ     350
#define SCALE125HZ    809
#define SCALE250HZ    2260
#define SCALE500HZ    7847
#define SCALE1000HZ   16384
#define SCALE2000HZ   259686
#define SCALE4000HZ   163840
#define SCALE8000HZ   1294
#define QVALUE        16384
#define LEDLENGTH     49
#define LEDON         5
#define LEDOFF        0

// debug flags



//===========external global variables==================//

// audio input buffer
volatile int16      audioBuff[2][BUFFLEN]   = { {0},{0} };
volatile uint8      buffInput               = 0;
volatile uint8      buffOutput              = 1;
volatile uint16     buffIndex               = 0; 
volatile uint8      buffFull                = 0;


// function prototypes
short signed int mean( short signed int [] );
short unsigned int stdDev( short signed int [], int );



int main(void)
{
    // led thresholds 
    //uint16 thrshldUP[8]   = { 32, 50, 75, 116, 178, 273, 422, 650 };
    //uint16 thrshldDOWN[8] = { 19, 30, 45, 70, 107, 164, 253, 390 };
    uint16 thrshldUP[8]   = { 650, 422, 273, 178, 116, 75, 50, 32 };
    uint16 thrshldDOWN[8] = { 390, 253, 164, 107, 70, 45, 30, 19}; 
    
    // A-Scale weights
    unsigned int A_scale[8] = {SCALE63HZ, SCALE125HZ, SCALE250HZ, SCALE500HZ, SCALE1000HZ, SCALE2000HZ, SCALE4000HZ, SCALE8000HZ};
    
    
    //======== variables ================//
    short int fftData[2][BUFFLEN];
    int16 fftIndex;
    char buttonState = 0;
    char uartBuffer[48] = {0};
    char index = 0;
    char index2 = 0;
    char sensitivityButtState = 0;
    char sensitivityState = 0;
    int32 gain = 16384;
    int32 gainTemp = 0;
    char buttonUp = 1;
    char buttonDown = 1;
    char buttonUpPressTime = 0;
    char buttonDownPressTime = 0;
    signed int avg = 0;
    signed int std = 0;
    char index_threshold;
    
    // FFT freq resolution ~ 40Hz 41666/1024
    // guard band of at least 1 frequency bin is added to 
    // reduce effect of spectral leakage
    
    // 63 Hz -- 40 - 95
    short int freqBand1_LL = 1;
    short int freqBand1_UL = 2;
    
    // 125 Hz -- 95 - 188
    short int freqBand2_LL = 3;
    short int freqBand2_UL = 4;
    
    // 250 Hz -- 188 - 375
    short int freqBand3_LL = 5;
    short int freqBand3_UL = 9;
    
    // 500 Hz -- 375 - 750
    short int freqBand4_LL = 10;
    short int freqBand4_UL = 18;
    
    // 1000 Hz -- 750 - 1500
    short int freqBand5_LL = 19;
    short int freqBand5_UL = 37;
    
    // 2000 Hz -- 1500 - 3000
    short int freqBand6_LL = 38;
    short int freqBand6_UL = 75;
    
    // 4000 Hz -- 3000 - 6000
    short int freqBand7_LL = 76;
    short int freqBand7_UL = 150;
    
    // 8000 Hz -- 6000 - 12000
    short int freqBand8_LL = 151;
    short int freqBand8_UL = 300;
    
    // instantiate led objects which is an array of structs.
    led led_objects[LEDLENGTH];

    for(int index = 0; index < LEDLENGTH; index++)
    {
        led_init(&led_objects[index], LEDON, 230, 30, 130);      
    }
    
    // create led_objects buffer
    char display_buffer[LEDLENGTH * 4 + 8];
    update_buffer(display_buffer, led_objects, LEDLENGTH);
    
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
    Pin_OE_Write( 1 );
    Pin_debug1_Write( 0 );
    
    //DVDAC_1_Start();
            
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_PutString("COM port Open\r\n");
    
    // turn on display
    SPI_led_PutArray(display_buffer, LEDLENGTH * 4 + 8);
    
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
            Pin_debug1_Write( 1 );
            buffFull = 0;
            
            buttonUp = Pin_buttUp_Read();
            buttonDown = Pin_buttDown_Read();
            
            if( buttonUp == 0 )
            {
                buttonUpPressTime = buttonUpPressTime + 1;
            }
            else if( buttonDown == 0 )
            {
                buttonDownPressTime = buttonDownPressTime + 1;   
            }
            
            // adjust volume sensitivity by scaling the thresholds when user presses up button or down button
            if( buttonUpPressTime >= 5 )
            {
                buttonUpPressTime = 0;    
                
                if( sensitivityState < 32 )
                {
                    sensitivityState = sensitivityState + 1; 
                           
                    // 1.2 * 2^14 - 2^14 = 3276
                    gainTemp = gain;
                    gain = gain + 3276;
                    if( ( gain - gainTemp ) != 3276 )
                    {
                        // then variable has been corrupted, possibly during intterupt
                        gain = gainTemp + 3276;
                    }
                        
                }
            }
            else if( buttonDownPressTime >= 5 )
            {
                buttonDownPressTime = 0;
                
                if( sensitivityState > 0 )
                {
                    sensitivityState = sensitivityState - 1; 
                            
                    // 0.8 * 2^14 - 2^14 = 15565
                    gainTemp = gain;
                    gain = gain - 3276;
                    if( ( gainTemp - gain ) != 3276 )
                    {
                        // then variable has been corrupted, possibly during intterupt
                        gain = gainTemp - 3276;
                    }
                }  
                
            }
            
            
            // Copy audio buffer to complex valued buffer and multiply with hamming window
            
            for( fftIndex = 0; fftIndex < BUFFLEN; fftIndex++ )
            {
                fftData[REAL][fftIndex] = ( audioBuff[ buffOutput ][ fftIndex ] * hammingWin[fftIndex] ) >> QFACTOR;
            }
            
            // calculate FFT
            fft( fftData );
            
            // calculate RMS of several frequency bands from freq spectrum and compare to thresholds
            // use hysteresis to avoid led's flickering
            std = fftRMS( fftData, freqBand1_LL, freqBand1_UL );
            std = std * SCALE63HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 0; index < 7; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                
            }
            
            std = fftRMS( fftData, freqBand2_LL, freqBand2_UL );
            std = std * SCALE125HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 7; index < 14; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
            }
            
            std = fftRMS( fftData, freqBand3_LL, freqBand3_UL );
            std = std * SCALE250HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 14; index < 21; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
            }
            
            std = fftRMS( fftData, freqBand4_LL, freqBand4_UL );
            std = std * SCALE500HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 21; index < 28; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
            }
            
            std = fftRMS( fftData, freqBand5_LL, freqBand5_UL );
            std = std * SCALE1000HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 28; index < 35; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
            }
            
            std = fftRMS( fftData, freqBand6_LL, freqBand6_UL );
            std = std * SCALE2000HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 35; index < 42; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
            }
            
            std = fftRMS( fftData, freqBand7_LL, freqBand7_UL );
            std = std * SCALE4000HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            index_threshold = 0;
            for( index = 42; index < 49; index++ )
            {
                if( led_brightness_read(&led_objects[index]) > LEDOFF )
                {   
                    // if led was ON during previous output, use lower threshold
                    if( std > thrshldDOWN[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    }     
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                else
                {
                    // if led was OFF during previous output, use upper threshold
                    if( std > thrshldUP[ index_threshold++ ] )
                    {
                        // if audio level is greater then threshold turn on led
                        led_brightness_write(&led_objects[index], LEDON);
                    } 
                    else
                    {
                        led_brightness_write(&led_objects[index], LEDOFF);   
                    }
                }
                
            }
/* need to update this last part once i have 8 columns of leds            
            std = fftRMS( fftData, freqBand8_LL, freqBand8_UL );
            std = std * SCALE8000HZ >> QFACTOR;
            std = gain * std >> QFACTOR;
            for( index = 0; index < 8; index++ )
            {
                if( ledsOut[7] & (1<<index) )
                {   
                    // if led was set on previous output use lower threshold
                    if( std > thrshldDOWN[ index ] )
                    {
                        ledVal = ledVal | ( 1 << index );
                    }     
                }
                else
                {
                    // if led was NOT set on previous output use upper threshold
                    if( std > thrshldUP[ index ] )
                    {
                        ledVal = ledVal | ( 1 << index );
                    } 
                }
                
            }

*/            

            // If buttonState == 1 write LED output values to shift registers via SPI
            if( buttonState )
            {
                update_buffer(display_buffer, led_objects, LEDLENGTH);
                SPI_led_PutArray(display_buffer, LEDLENGTH * 4 + 8);
                
                sprintf( uartBuffer, "Gain value: %d\r\n", gain );
                UART_1_PutString(uartBuffer);

            }
            else
            {
                for(index = 0; index < LEDLENGTH; index++)
                {
                    led_brightness_write(&led_objects[index], LEDOFF);   
                }
                update_buffer(display_buffer, led_objects, LEDLENGTH );
                SPI_led_PutArray(display_buffer, LEDLENGTH * 4 + 8);                
            }
            
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



/* [] END OF FILE */
