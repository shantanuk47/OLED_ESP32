/***************************************************************
 * font_data.c
 * 8x8 Font Data Implementation for OLED Display
 *
 * Project: VDU_ESP32 (Vehicle Display Unit)
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

/* Include necessary headers */
#include "font_data.h"
#include <stdbool.h>
#include <stddef.h>

/* Private function declarations */
static bool font_get_char_data_internal(char c, uint8_t* font_data);

/* Function implementations */

bool font_get_char_data(char c, uint8_t* font_data)
{
    bool result = false;
    
    if (font_data != NULL)
    {
        result = font_get_char_data_internal(c, font_data);
    }
    
    return result;
}

bool font_is_char_supported(char c)
{
    uint8_t dummy_data[FONT_BYTES_PER_CHAR];
    return font_get_char_data_internal(c, dummy_data);
}

static bool font_get_char_data_internal(char c, uint8_t* font_data)
{
    bool result = true;
    
    switch (c) 
    {
        case 'T': 
            font_data[0] = 0x3F; font_data[1] = 0x2D; font_data[2] = 0x0C; 
            font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case 'E': 
            font_data[0] = 0x7F; font_data[1] = 0x46; font_data[2] = 0x16; 
            font_data[3] = 0x1E; font_data[4] = 0x16; font_data[5] = 0x46; 
            font_data[6] = 0x7F; font_data[7] = 0x00; 
            break;
        case 'S': 
            font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x07; 
            font_data[3] = 0x0E; font_data[4] = 0x38; font_data[5] = 0x33; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case '1': 
            font_data[0] = 0x0C; font_data[1] = 0x0E; font_data[2] = 0x0C; 
            font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; 
            font_data[6] = 0x3F; font_data[7] = 0x00; 
            break;
        case '2': 
            font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x30; 
            font_data[3] = 0x1C; font_data[4] = 0x06; font_data[5] = 0x33; 
            font_data[6] = 0x3F; font_data[7] = 0x00; 
            break;
        case '3': 
            font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x30; 
            font_data[3] = 0x1C; font_data[4] = 0x30; font_data[5] = 0x33; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case '4': 
            font_data[0] = 0x38; font_data[1] = 0x3C; font_data[2] = 0x36; 
            font_data[3] = 0x33; font_data[4] = 0x7F; font_data[5] = 0x30; 
            font_data[6] = 0x78; font_data[7] = 0x00; 
            break;
        case 'A': 
            font_data[0] = 0x0C; font_data[1] = 0x1E; font_data[2] = 0x33; 
            font_data[3] = 0x33; font_data[4] = 0x3F; font_data[5] = 0x33; 
            font_data[6] = 0x33; font_data[7] = 0x00; 
            break;
        case 'B': 
            font_data[0] = 0x3F; font_data[1] = 0x66; font_data[2] = 0x66; 
            font_data[3] = 0x3E; font_data[4] = 0x66; font_data[5] = 0x66; 
            font_data[6] = 0x3F; font_data[7] = 0x00; 
            break;
        case 'C': 
            font_data[0] = 0x3C; font_data[1] = 0x66; font_data[2] = 0x03; 
            font_data[3] = 0x03; font_data[4] = 0x03; font_data[5] = 0x66; 
            font_data[6] = 0x3C; font_data[7] = 0x00; 
            break;
        case 'D': 
            font_data[0] = 0x1F; font_data[1] = 0x36; font_data[2] = 0x66; 
            font_data[3] = 0x66; font_data[4] = 0x66; font_data[5] = 0x36; 
            font_data[6] = 0x1F; font_data[7] = 0x00; 
            break;
        case 'H': 
            font_data[0] = 0x33; font_data[1] = 0x33; font_data[2] = 0x33; 
            font_data[3] = 0x3F; font_data[4] = 0x33; font_data[5] = 0x33; 
            font_data[6] = 0x33; font_data[7] = 0x00; 
            break;
        case 'e': 
            font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x1E; 
            font_data[3] = 0x33; font_data[4] = 0x3F; font_data[5] = 0x03; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case 'l': 
            font_data[0] = 0x0E; font_data[1] = 0x0C; font_data[2] = 0x0C; 
            font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case 'o': 
            font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x1E; 
            font_data[3] = 0x33; font_data[4] = 0x33; font_data[5] = 0x33; 
            font_data[6] = 0x1E; font_data[7] = 0x00; 
            break;
        case ' ': 
            font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x00; 
            font_data[3] = 0x00; font_data[4] = 0x00; font_data[5] = 0x00; 
            font_data[6] = 0x00; font_data[7] = 0x00; 
            break;
        case 'W': 
            font_data[0] = 0x63; font_data[1] = 0x63; font_data[2] = 0x63; 
            font_data[3] = 0x6B; font_data[4] = 0x7F; font_data[5] = 0x77; 
            font_data[6] = 0x63; font_data[7] = 0x00; 
            break;
        case 'r': 
            font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x3B; 
            font_data[3] = 0x6E; font_data[4] = 0x66; font_data[5] = 0x06; 
            font_data[6] = 0x0F; font_data[7] = 0x00; 
            break;
        case 'd': 
            font_data[0] = 0x38; font_data[1] = 0x30; font_data[2] = 0x30; 
            font_data[3] = 0x3E; font_data[4] = 0x33; font_data[5] = 0x33; 
            font_data[6] = 0x6E; font_data[7] = 0x00; 
            break;
        case '!': 
            font_data[0] = 0x18; font_data[1] = 0x3C; font_data[2] = 0x3C; 
            font_data[3] = 0x18; font_data[4] = 0x18; font_data[5] = 0x00; 
            font_data[6] = 0x18; font_data[7] = 0x00; 
            break;
        case 'P': 
            font_data[0] = 0x3F; font_data[1] = 0x66; font_data[2] = 0x66; 
            font_data[3] = 0x3E; font_data[4] = 0x06; font_data[5] = 0x06; 
            font_data[6] = 0x0F; font_data[7] = 0x00; 
            break;
        case 'L': 
            font_data[0] = 0x0F; font_data[1] = 0x06; font_data[2] = 0x06; 
            font_data[3] = 0x06; font_data[4] = 0x46; font_data[5] = 0x66; 
            font_data[6] = 0x7F; font_data[7] = 0x00; 
            break;
        default: 
            /* Use space character for unsupported characters */
            font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x00; 
            font_data[3] = 0x00; font_data[4] = 0x00; font_data[5] = 0x00; 
            font_data[6] = 0x00; font_data[7] = 0x00; 
            result = false;
            break;
    }
    
    return result;
}
