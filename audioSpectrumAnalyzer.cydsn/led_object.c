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
#include    <led_object.h>

// define the functions
void led_init(led *data, char brightness, char blue, char green, char red)
{
    data->brightness = 0xe0 | brightness;
    data->blue = blue;
    data->green = green;
    data->red = red;
}


char led_id_read(led *data)
{
    return data-> id;
}

void led_id_write(led *data, char id)
{
    data->id = id;
}

char led_brightness_read(led *data)
{
    return (data->brightness & 0x1f);
}

void led_brightness_write(led *data, char brightness)
{
    data->brightness = 0xe0 | brightness;
}

char led_blue_read(led *data)
{
    return data->blue;
}

void led_blue_write(led *data, char value)
{
    data->blue = value;
}

char led_green_read(led *data)
{
    return data->green;
}

void led_green_write(led *data, char value)
{
     data->green = value;
}

char led_red_read(led *data)
{
    return data->red;
}

void led_red_write(led *data, char value)
{
    data->red = value;
}

/* [] END OF FILE */
