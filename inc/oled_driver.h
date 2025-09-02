/***************************************************************
 * oled_driver.h
 * SSD1306 OLED Display Driver Header File
 *
 * This header defines the interface for controlling the 0.96" SSD1306
 * OLED display (128x64 pixels) via I2C communication. It provides
 * functions for display initialization, text rendering, graphics
 * drawing, and display control operations.
 *
 * Features:
 * - I2C communication with SSD1306 controller
 * - Text display with custom 8x8 font
 * - Graphics functions (pixel, line, rectangle)
 * - Display orientation control
 * - Memory management and cursor control
 *
 * Hardware: 0.96" SSD1306 OLED Display (128x64)
 * Interface: I2C (SDA: GPIO21, SCL: GPIO22)
 * Address: 0x3C (default)
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

/* Include necessary headers */
#include "esp_err.h"
#include "driver/i2c.h"
#include "pin_config.h"
#include <stdint.h>
#include <stdbool.h>

/* MISRA C Compliance */
#ifdef __cplusplus
extern "C" {
#endif

/* Constants and Macros */
#define OLED_ADDR                0x3C
#define OLED_WIDTH               128
#define OLED_HEIGHT              64
#define OLED_PAGES               8
#define OLED_COLUMNS             128

/* I2C Configuration - Defined in pin_config.h */

/* SSD1306 Commands */
#define OLED_CMD_DISPLAY_OFF     0xAE
#define OLED_CMD_DISPLAY_ON      0xAF
#define OLED_CMD_SET_DISPLAY_CLK_DIV 0xD5
#define OLED_CMD_SET_MULTIPLEX   0xA8
#define OLED_CMD_SET_DISPLAY_OFFSET 0xD3
#define OLED_CMD_SET_START_LINE  0x40
#define OLED_CMD_CHARGE_PUMP     0x8D
#define OLED_CMD_MEMORY_ADDR_MODE 0x20
#define OLED_CMD_SEG_REMAP       0xA0
#define OLED_CMD_COM_SCAN_INC    0xC0
#define OLED_CMD_COM_SCAN_DEC    0xC8
#define OLED_CMD_SET_COM_PINS    0xDA
#define OLED_CMD_SET_CONTRAST    0x81
#define OLED_CMD_SET_PRECHARGE   0xD9
#define OLED_CMD_SET_VCOM_DETECT 0xDB
#define OLED_CMD_DISPLAY_ALL_ON_RESUME 0xA4
#define OLED_CMD_NORMAL_DISPLAY  0xA6
#define OLED_CMD_DEACTIVATE_SCROLL 0x2E
#define OLED_CMD_ACTIVATE_SCROLL 0x2F
#define OLED_CMD_SET_VERTICAL_SCROLL_AREA 0xA3

/* Function Declarations */

/**
 * @brief Initialize I2C master
 * @return ESP_OK on success, error code on failure
 */
esp_err_t i2c_master_init(void);

/**
 * @brief Initialize OLED display
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_init(void);

/**
 * @brief Write command to OLED
 * @param cmd Command byte to send
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_write_cmd(uint8_t cmd);

/**
 * @brief Write data to OLED
 * @param data Data byte to send
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_write_data(uint8_t data);

/**
 * @brief Clear entire display
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_clear(void);

/**
 * @brief Set cursor position for text
 * @param x Column position (0-15 for 8x8 font)
 * @param y Row position (0-7 for 8x8 font)
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_set_cursor(uint8_t x, uint8_t y);

/**
 * @brief Write single character to display
 * @param c Character to display
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_write_char(char c);

/**
 * @brief Write string to display
 * @param str Null-terminated string to display
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_write_string(const char* str);

/**
 * @brief Draw single pixel
 * @param x X coordinate (0-127)
 * @param y Y coordinate (0-63)
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_draw_pixel(uint8_t x, uint8_t y);

/**
 * @brief Draw line between two points
 * @param x1 Start X coordinate
 * @param y1 Start Y coordinate
 * @param x2 End X coordinate
 * @param y2 End Y coordinate
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * @brief Draw rectangle outline
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#ifdef __cplusplus
}
#endif

#endif /* OLED_DRIVER_H */
