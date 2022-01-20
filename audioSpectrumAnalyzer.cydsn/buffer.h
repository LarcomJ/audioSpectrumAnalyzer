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
#if !defined( BUFFER_HEADER )
#define BUFFER_HEADER

#include "cytypes.h"

#define Fs              41666u
#define FRAMERATE       20
#define BUFFLEN         2048    // closestPowOf2( (Fs/FRAMERATE) )
#define LOG2_BUFFLEN    10

// audio input buffer
extern volatile int16      audioBuff[2][BUFFLEN];
extern volatile uint8      buffInput;
extern volatile uint8      buffOutput;
extern volatile uint16     buffIndex; 
extern volatile uint8      buffFull;

#endif

/* [] END OF FILE */
