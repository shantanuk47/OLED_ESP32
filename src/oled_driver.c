/***************************************************************
 * oled_driver.c
 * SSD1306 OLED Display Driver Implementation
 *
 * This module implements the complete driver for the 0.96" SSD1306
 * OLED display (128x64 pixels). It handles I2C communication,
 * display initialization, text rendering with custom font rotation,
 * graphics drawing, and display control operations.
 *
 * Key Functions:
 * - I2C master initialization and communication
 * - OLED display initialization with proper orientation
 * - Text rendering with character rotation and mirroring
 * - Graphics primitives (pixel, line, rectangle)
 * - Display memory management and cursor control
 *
 * The driver includes special font rendering algorithms to handle
 * character orientation for proper display on the OLED screen.
 *
 * Hardware: 0.96" SSD1306 OLED Display (128x64)
 * Interface: I2C (SDA: GPIO21, SCL: GPIO22, Address: 0x3C)
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

/* Include necessary headers */
#include "oled_driver.h"
#include "font_data.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

/* Private constants */
static const char* TAG = "OLED_DRIVER";

/* Private variables */
static uint8_t g_cursor_x = 0;
static uint8_t g_cursor_y = 0;

/* Private function declarations */
static esp_err_t oled_write_byte(uint8_t data, bool is_cmd);
static esp_err_t oled_set_page_address(uint8_t page);
static esp_err_t oled_set_column_address(uint8_t col);

/* Function implementations */

esp_err_t i2c_master_init(void)
{
    esp_err_t ret = ESP_OK;
    i2c_config_t conf = {0};
    
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    
    ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C param config failed: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 
                            I2C_MASTER_RX_BUF_DISABLE, 
                            I2C_MASTER_TX_BUF_DISABLE, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(ret));
    }
    
    return ret;
}

esp_err_t oled_init(void)
{
    esp_err_t ret = ESP_OK;
    
    /* Power on delay */
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    /* Fundamental commands */
    ret |= oled_write_cmd(OLED_CMD_DISPLAY_OFF);
    ret |= oled_write_cmd(OLED_CMD_SET_DISPLAY_CLK_DIV);
    ret |= oled_write_cmd(0x80);
    ret |= oled_write_cmd(OLED_CMD_SET_MULTIPLEX);
    ret |= oled_write_cmd(0x3F);
    ret |= oled_write_cmd(OLED_CMD_SET_DISPLAY_OFFSET);
    ret |= oled_write_cmd(0x00);
    ret |= oled_write_cmd(OLED_CMD_SET_START_LINE | 0x0);
    ret |= oled_write_cmd(OLED_CMD_CHARGE_PUMP);
    ret |= oled_write_cmd(0x14);
    ret |= oled_write_cmd(OLED_CMD_MEMORY_ADDR_MODE);
    ret |= oled_write_cmd(0x00);
    ret |= oled_write_cmd(OLED_CMD_SEG_REMAP | 0x1);
    ret |= oled_write_cmd(OLED_CMD_COM_SCAN_DEC);
    ret |= oled_write_cmd(OLED_CMD_SET_COM_PINS);
    ret |= oled_write_cmd(0x12);
    ret |= oled_write_cmd(OLED_CMD_SET_CONTRAST);
    ret |= oled_write_cmd(0xCF);
    ret |= oled_write_cmd(OLED_CMD_SET_PRECHARGE);
    ret |= oled_write_cmd(0xF1);
    ret |= oled_write_cmd(OLED_CMD_SET_VCOM_DETECT);
    ret |= oled_write_cmd(0x40);
    ret |= oled_write_cmd(OLED_CMD_DISPLAY_ALL_ON_RESUME);
    ret |= oled_write_cmd(OLED_CMD_NORMAL_DISPLAY);
    ret |= oled_write_cmd(OLED_CMD_DEACTIVATE_SCROLL);
    ret |= oled_write_cmd(OLED_CMD_DISPLAY_ON);
    
    /* Initialization delay */
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "OLED initialization failed");
    }
    else
    {
        ESP_LOGI(TAG, "OLED initialized successfully");
    }
    
    return ret;
}

esp_err_t oled_write_cmd(uint8_t cmd)
{
    return oled_write_byte(cmd, true);
}

esp_err_t oled_write_data(uint8_t data)
{
    return oled_write_byte(data, false);
}

esp_err_t oled_clear(void)
{
    esp_err_t ret = ESP_OK;
    uint8_t page;
    uint8_t col;
    
    for (page = 0; page < OLED_PAGES; page++)
    {
        ret |= oled_set_page_address(page);
        ret |= oled_set_column_address(0);
        
        for (col = 0; col < OLED_COLUMNS; col++)
        {
            ret |= oled_write_data(0x00);
        }
    }
    
    /* Reset cursor position */
    g_cursor_x = 0;
    g_cursor_y = 0;
    
    return ret;
}

esp_err_t oled_set_cursor(uint8_t x, uint8_t y)
{
    esp_err_t ret = ESP_OK;
    
    if ((x < 16) && (y < 8))
    {
        g_cursor_x = x;
        g_cursor_y = y;
        
        ret |= oled_set_page_address(y);
        ret |= oled_set_column_address(x * FONT_WIDTH);
    }
    else
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    
    return ret;
}

esp_err_t oled_write_char(char c)
{
    esp_err_t ret = ESP_OK;
    uint8_t font_data[FONT_BYTES_PER_CHAR];
    uint8_t i;
    uint8_t j;
    
    /* Get font data for character */
    if (!font_get_char_data(c, font_data))
    {
        /* Use space for unsupported characters */
        font_get_char_data(' ', font_data);
    }
    
    /* Mirror the font data horizontally first, then rotate 90 degrees counter-clockwise */
    for (i = 0; i < FONT_HEIGHT; i++)
    {
        uint8_t rotated_byte = 0;
        for (j = 0; j < FONT_WIDTH; j++)
        {
            if (font_data[j] & (1U << i))
            {
                rotated_byte |= (1U << j);
            }
        }
        ret |= oled_write_data(rotated_byte);
    }
    
    /* Advance cursor */
    g_cursor_x++;
    if (g_cursor_x >= 16)
    {
        g_cursor_x = 0;
        g_cursor_y++;
        if (g_cursor_y >= 8)
        {
            g_cursor_y = 0;
        }
        ret |= oled_set_cursor(g_cursor_x, g_cursor_y);
    }
    
    return ret;
}

esp_err_t oled_write_string(const char* str)
{
    esp_err_t ret = ESP_OK;
    
    if (str != NULL)
    {
        while (*str != '\0')
        {
            ret |= oled_write_char(*str);
            str++;
        }
    }
    else
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    
    return ret;
}

esp_err_t oled_draw_pixel(uint8_t x, uint8_t y)
{
    esp_err_t ret = ESP_OK;
    uint8_t page;
    uint8_t col;
    uint8_t bit;
    uint8_t data;
    
    if ((x < OLED_WIDTH) && (y < OLED_HEIGHT))
    {
        page = y / 8;
        col = x;
        bit = y % 8;
        
        ret |= oled_set_page_address(page);
        ret |= oled_set_column_address(col);
        
        /* Read current data, set bit, write back */
        data = 1U << bit;
        ret |= oled_write_data(data);
    }
    else
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    
    return ret;
}

esp_err_t oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    esp_err_t ret = ESP_OK;
    int16_t dx;
    int16_t dy;
    int16_t sx;
    int16_t sy;
    int16_t err;
    int16_t e2;
    int16_t x;
    int16_t y;
    
    dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;
    
    x = x1;
    y = y1;
    
    while (true)
    {
        ret |= oled_draw_pixel((uint8_t)x, (uint8_t)y);
        
        if ((x == x2) && (y == y2))
        {
            break;
        }
        
        e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
    
    return ret;
}

esp_err_t oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    esp_err_t ret = ESP_OK;
    
    /* Draw top and bottom lines */
    ret |= oled_draw_line(x, y, x + w - 1, y);
    ret |= oled_draw_line(x, y + h - 1, x + w - 1, y + h - 1);
    
    /* Draw left and right lines */
    ret |= oled_draw_line(x, y, x, y + h - 1);
    ret |= oled_draw_line(x + w - 1, y, x + w - 1, y + h - 1);
    
    return ret;
}

/* Private function implementations */

static esp_err_t oled_write_byte(uint8_t data, bool is_cmd)
{
    esp_err_t ret = ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    if (cmd != NULL)
    {
        ret = i2c_master_start(cmd);
        if (ret == ESP_OK)
        {
            ret = i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
        }
        if (ret == ESP_OK)
        {
            ret = i2c_master_write_byte(cmd, is_cmd ? 0x00 : 0x40, true);
        }
        if (ret == ESP_OK)
        {
            ret = i2c_master_write_byte(cmd, data, true);
        }
        if (ret == ESP_OK)
        {
            ret = i2c_master_stop(cmd);
        }
        if (ret == ESP_OK)
        {
            ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 
                                     pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
        }
        
        i2c_cmd_link_delete(cmd);
    }
    else
    {
        ret = ESP_ERR_NO_MEM;
    }
    
    return ret;
}

static esp_err_t oled_set_page_address(uint8_t page)
{
    esp_err_t ret = ESP_OK;
    
    if (page < OLED_PAGES)
    {
        ret |= oled_write_cmd(0xB0 | page);
    }
    else
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    
    return ret;
}

static esp_err_t oled_set_column_address(uint8_t col)
{
    esp_err_t ret = ESP_OK;
    
    if (col < OLED_COLUMNS)
    {
        ret |= oled_write_cmd(0x10 | (col >> 4));
        ret |= oled_write_cmd(0x00 | (col & 0x0F));
    }
    else
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    
    return ret;
}
