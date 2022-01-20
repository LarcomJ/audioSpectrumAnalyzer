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
#include    <display.h>
#include    "cytypes.h"

// define functions
void update_buffer(uint8 *buff, led array[], char len )
{
    //unsigned char byteLen = 4*len;
    
    // fill the buffer with the start frame
    *buff++ = 0u;
    *buff++ = 0u;
    *buff++ = 0u;
    *buff++ = 0u;
    
    // loop through each led object and copy info into buffer
    // because the leds are serially driven the first data
    // sent out controls the last led so, we need to send the 
    // led info in reverse order.
    //for(int index = len-1; index >= 0; index--)
    for(int index = 0; index < len; index++)
    {
        *buff++ = array[index].brightness; 
        *buff++ = array[index].blue;
        *buff++ = array[index].green;
        *buff++ = array[index].red;
    }
    
    // fill the buffer with the end frame
    *buff++ = 0xff;
    *buff++ = 0xff;
    *buff++ = 0xff;
    *buff++ = 0xff;   
    
}

/* [] END OF FILE */
