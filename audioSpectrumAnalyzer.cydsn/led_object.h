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
#ifndef LED
#define LED

    // define the data structure for the led object
    typedef struct {    char id;
                        char brightness;
                        char blue;
                        char green;
                        char red; } led;
    
    // prototype the functions
    void led_init(led *data, char brightness, char blue, char green, char red);
    char led_id_read(led *data);
    void led_id_write(led *data,char);
    char led_brightness_read(led *data);
    void led_brightness_write(led *data, char brightness);
    char led_blue_read(led *data);
    void led_blue_write(led *data, char value);
    char led_green_read(led *data);
    void led_green_write(led *data, char value);
    char led_red_read(led *data);
    void led_red_write(led *data, char value);
    
#endif
/* [] END OF FILE */
