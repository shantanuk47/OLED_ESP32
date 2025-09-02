/***************************************************************
 * font_data.h
 * 8x8 Font Data Definitions for OLED Display
 *
 * Project: VDU_ESP32 (Vehicle Display Unit)
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

#ifndef FONT_DATA_H
#define FONT_DATA_H

/* Include necessary headers */
#include <stdint.h>
#include <stdbool.h>

/* MISRA C Compliance */
#ifdef __cplusplus
extern "C" {
#endif

/* Font Constants */
#define FONT_WIDTH               8
#define FONT_HEIGHT              8
#define FONT_BYTES_PER_CHAR      8

/* Function Declarations */

/**
 * @brief Get font data for a specific character
 * @param c Character to get font data for
 * @param font_data Pointer to array to store font data (8 bytes)
 * @return true if character found, false otherwise
 */
bool font_get_char_data(char c, uint8_t* font_data);

/**
 * @brief Check if character is supported
 * @param c Character to check
 * @return true if supported, false otherwise
 */
bool font_is_char_supported(char c);

#ifdef __cplusplus
}
#endif

#endif /* FONT_DATA_H */
