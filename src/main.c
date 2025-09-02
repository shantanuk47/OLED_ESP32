#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"

// OLED Configuration
#define I2C_MASTER_SCL_IO           22    /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           21    /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0     /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000 /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0     /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0     /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

// OLED Address (0x3C for most 0.96" OLEDs)
#define OLED_ADDR                    0x3C

// OLED Commands
#define OLED_CMD_SET_CONTRAST        0x81
#define OLED_CMD_DISPLAY_ALL_ON_RESUME 0xA4
#define OLED_CMD_DISPLAY_ALL_ON      0xA5
#define OLED_CMD_NORMAL_DISPLAY      0xA6
#define OLED_CMD_INVERT_DISPLAY      0xA7
#define OLED_CMD_DISPLAY_OFF         0xAE
#define OLED_CMD_DISPLAY_ON          0xAF
#define OLED_CMD_SET_DISPLAY_OFFSET  0xD3
#define OLED_CMD_SET_COM_PINS        0xDA
#define OLED_CMD_SET_VCOM_DETECT     0xDB
#define OLED_CMD_SET_DISPLAY_CLK_DIV 0xD5
#define OLED_CMD_SET_PRECHARGE       0xD9
#define OLED_CMD_SET_MULTIPLEX       0xA8
#define OLED_CMD_SET_LOW_COLUMN      0x00
#define OLED_CMD_SET_HIGH_COLUMN     0x10
#define OLED_CMD_SET_START_LINE      0x40
#define OLED_CMD_MEMORY_MODE         0x20
#define OLED_CMD_COLUMN_ADDR         0x21
#define OLED_CMD_PAGE_ADDR           0x22
#define OLED_CMD_COM_SCAN_INC        0xC0
#define OLED_CMD_COM_SCAN_DEC        0xC8
#define OLED_CMD_SEG_REMAP           0xA0
#define OLED_CMD_CHARGE_PUMP         0x8D

// OLED Dimensions
#define OLED_WIDTH                   128
#define OLED_HEIGHT                  64
#define OLED_PAGES                   8

static const char *TAG = "OLED_TEST";

// Function prototypes
static esp_err_t i2c_master_init(void);
static esp_err_t oled_write_cmd(uint8_t cmd);
static esp_err_t oled_write_data(uint8_t data);
static esp_err_t oled_init(void);
static esp_err_t oled_clear(void);
static esp_err_t oled_set_cursor(uint8_t x, uint8_t y);
static esp_err_t oled_write_char(char c);
static esp_err_t oled_write_string(const char *str);
static esp_err_t oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
static esp_err_t oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
static esp_err_t oled_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
static void oled_test_pattern(void);
static void oled_hello_world(void);

/**
 * @brief Initialize I2C master
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief Write command to OLED
 */
static esp_err_t oled_write_cmd(uint8_t cmd)
{
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_handle, 0x00, true); // Command mode
    i2c_master_write_byte(cmd_handle, cmd, true);
    i2c_master_stop(cmd_handle);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_handle);
    return ret;
}

/**
 * @brief Write data to OLED
 */
static esp_err_t oled_write_data(uint8_t data)
{
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_handle, 0x40, true); // Data mode
    i2c_master_write_byte(cmd_handle, data, true);
    i2c_master_stop(cmd_handle);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_handle);
    return ret;
}

/**
 * @brief Initialize OLED display
 */
static esp_err_t oled_init(void)
{
    ESP_LOGI(TAG, "Initializing OLED display...");
    
    // Simple initialization sequence for SSD1306
    esp_err_t ret = ESP_OK;
    
    vTaskDelay(100 / portTICK_PERIOD_MS); // Wait for power stabilization
    
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
    ret |= oled_write_cmd(OLED_CMD_MEMORY_MODE);
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
    ret |= oled_write_cmd(OLED_CMD_DISPLAY_ON);
    
    vTaskDelay(100 / portTICK_PERIOD_MS); // Wait after initialization
    
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OLED initialized successfully");
    } else {
        ESP_LOGE(TAG, "OLED initialization failed");
    }
    
    return ret;
}

/**
 * @brief Clear OLED display
 */
static esp_err_t oled_clear(void)
{
    esp_err_t ret = ESP_OK;
    
    for (uint8_t page = 0; page < OLED_PAGES; page++) {
        ret |= oled_write_cmd(OLED_CMD_PAGE_ADDR);
        ret |= oled_write_cmd(page);
        ret |= oled_write_cmd(page);
        ret |= oled_write_cmd(OLED_CMD_COLUMN_ADDR);
        ret |= oled_write_cmd(0);
        ret |= oled_write_cmd(OLED_WIDTH - 1);
        
        for (uint8_t col = 0; col < OLED_WIDTH; col++) {
            ret |= oled_write_data(0x00);
        }
    }
    
    return ret;
}

/**
 * @brief Set cursor position
 */
static esp_err_t oled_set_cursor(uint8_t x, uint8_t y)
{
    esp_err_t ret = ESP_OK;
    
    ret |= oled_write_cmd(OLED_CMD_PAGE_ADDR);
    ret |= oled_write_cmd(y);
    ret |= oled_write_cmd(y);
    ret |= oled_write_cmd(OLED_CMD_COLUMN_ADDR);
    ret |= oled_write_cmd(x);
    ret |= oled_write_cmd(OLED_WIDTH - 1);
    
    return ret;
}

/**
 * @brief Write a character to OLED (simple 8x8 font)
 */
static esp_err_t oled_write_char(char c)
{
    // Simple font for basic characters we need
    uint8_t font_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // default space
    
    // Basic character patterns for Hello World
    switch (c) {
        case 'T': font_data[0] = 0x3F; font_data[1] = 0x2D; font_data[2] = 0x0C; font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case 'E': font_data[0] = 0x7F; font_data[1] = 0x46; font_data[2] = 0x16; font_data[3] = 0x1E; font_data[4] = 0x16; font_data[5] = 0x46; font_data[6] = 0x7F; font_data[7] = 0x00; break;
        case 'S': font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x07; font_data[3] = 0x0E; font_data[4] = 0x38; font_data[5] = 0x33; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case '1': font_data[0] = 0x0C; font_data[1] = 0x0E; font_data[2] = 0x0C; font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; font_data[6] = 0x3F; font_data[7] = 0x00; break;
        case '2': font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x30; font_data[3] = 0x1C; font_data[4] = 0x06; font_data[5] = 0x33; font_data[6] = 0x3F; font_data[7] = 0x00; break;
        case '3': font_data[0] = 0x1E; font_data[1] = 0x33; font_data[2] = 0x30; font_data[3] = 0x1C; font_data[4] = 0x30; font_data[5] = 0x33; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case '4': font_data[0] = 0x38; font_data[1] = 0x3C; font_data[2] = 0x36; font_data[3] = 0x33; font_data[4] = 0x7F; font_data[5] = 0x30; font_data[6] = 0x78; font_data[7] = 0x00; break;
        case 'A': font_data[0] = 0x0C; font_data[1] = 0x1E; font_data[2] = 0x33; font_data[3] = 0x33; font_data[4] = 0x3F; font_data[5] = 0x33; font_data[6] = 0x33; font_data[7] = 0x00; break;
        case 'B': font_data[0] = 0x3F; font_data[1] = 0x66; font_data[2] = 0x66; font_data[3] = 0x3E; font_data[4] = 0x66; font_data[5] = 0x66; font_data[6] = 0x3F; font_data[7] = 0x00; break;
        case 'C': font_data[0] = 0x3C; font_data[1] = 0x66; font_data[2] = 0x03; font_data[3] = 0x03; font_data[4] = 0x03; font_data[5] = 0x66; font_data[6] = 0x3C; font_data[7] = 0x00; break;
        case 'D': font_data[0] = 0x1F; font_data[1] = 0x36; font_data[2] = 0x66; font_data[3] = 0x66; font_data[4] = 0x66; font_data[5] = 0x36; font_data[6] = 0x1F; font_data[7] = 0x00; break;
        case 'H': font_data[0] = 0x33; font_data[1] = 0x33; font_data[2] = 0x33; font_data[3] = 0x3F; font_data[4] = 0x33; font_data[5] = 0x33; font_data[6] = 0x33; font_data[7] = 0x00; break;
        case 'e': font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x1E; font_data[3] = 0x33; font_data[4] = 0x3F; font_data[5] = 0x03; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case 'l': font_data[0] = 0x0E; font_data[1] = 0x0C; font_data[2] = 0x0C; font_data[3] = 0x0C; font_data[4] = 0x0C; font_data[5] = 0x0C; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case 'o': font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x1E; font_data[3] = 0x33; font_data[4] = 0x33; font_data[5] = 0x33; font_data[6] = 0x1E; font_data[7] = 0x00; break;
        case ' ': font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x00; font_data[3] = 0x00; font_data[4] = 0x00; font_data[5] = 0x00; font_data[6] = 0x00; font_data[7] = 0x00; break;
        case 'W': font_data[0] = 0x63; font_data[1] = 0x63; font_data[2] = 0x63; font_data[3] = 0x6B; font_data[4] = 0x7F; font_data[5] = 0x77; font_data[6] = 0x63; font_data[7] = 0x00; break;
        case 'r': font_data[0] = 0x00; font_data[1] = 0x00; font_data[2] = 0x3B; font_data[3] = 0x6E; font_data[4] = 0x66; font_data[5] = 0x06; font_data[6] = 0x0F; font_data[7] = 0x00; break;
        case 'd': font_data[0] = 0x38; font_data[1] = 0x30; font_data[2] = 0x30; font_data[3] = 0x3E; font_data[4] = 0x33; font_data[5] = 0x33; font_data[6] = 0x6E; font_data[7] = 0x00; break;
        case '!': font_data[0] = 0x18; font_data[1] = 0x3C; font_data[2] = 0x3C; font_data[3] = 0x18; font_data[4] = 0x18; font_data[5] = 0x00; font_data[6] = 0x18; font_data[7] = 0x00; break;
        case 'P': font_data[0] = 0x3F; font_data[1] = 0x66; font_data[2] = 0x66; font_data[3] = 0x3E; font_data[4] = 0x06; font_data[5] = 0x06; font_data[6] = 0x0F; font_data[7] = 0x00; break;
        case 'L': font_data[0] = 0x0F; font_data[1] = 0x06; font_data[2] = 0x06; font_data[3] = 0x06; font_data[4] = 0x46; font_data[5] = 0x66; font_data[6] = 0x7F; font_data[7] = 0x00; break;
        default: break; // use default space
    }
    
    esp_err_t ret = ESP_OK;
    // Mirror the font data horizontally first, then rotate 90 degrees counter-clockwise
    for (int i = 0; i < 8; i++) {
        uint8_t rotated_byte = 0;
        for (int j = 0; j < 8; j++) {
            if (font_data[j] & (1 << i)) { // Use i instead of (7-i) to mirror horizontally
                rotated_byte |= (1 << j);
            }
        }
        ret |= oled_write_data(rotated_byte);
    }
    
    return ret;
}

/**
 * @brief Write string to OLED
 */
static esp_err_t oled_write_string(const char *str)
{
    esp_err_t ret = ESP_OK;
    while (*str && ret == ESP_OK) {
        ret = oled_write_char(*str++);
    }
    return ret;
}

/**
 * @brief Draw a pixel on OLED
 */
static esp_err_t oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return ESP_ERR_INVALID_ARG;
    
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    
    esp_err_t ret = oled_set_cursor(x, page);
    if (ret != ESP_OK) return ret;
    
    // Read current byte, modify bit, write back
    // For simplicity, we'll just set the pixel (this is a basic implementation)
    ret |= oled_write_data(1 << bit);
    
    return ret;
}

/**
 * @brief Draw a line on OLED
 */
static esp_err_t oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        oled_draw_pixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    
    return ESP_OK;
}

/**
 * @brief Draw a rectangle on OLED
 */
static esp_err_t oled_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    esp_err_t ret = ESP_OK;
    
    // Draw top and bottom lines
    ret |= oled_draw_line(x, y, x + width - 1, y, color);
    ret |= oled_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color);
    
    // Draw left and right lines
    ret |= oled_draw_line(x, y, x, y + height - 1, color);
    ret |= oled_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);
    
    return ret;
}

/**
 * @brief Test pattern for OLED
 */
static void oled_test_pattern(void)
{
    ESP_LOGI(TAG, "Drawing test pattern...");
    
    oled_clear();
    
    // Draw some basic shapes
    oled_draw_rectangle(10, 10, 50, 20, 1);
    oled_draw_rectangle(70, 10, 50, 20, 1);
    oled_draw_line(10, 40, 120, 40, 1);
    oled_draw_line(10, 50, 120, 50, 1);
    
    // Draw some pixels
    for (int i = 0; i < 10; i++) {
        oled_draw_pixel(20 + i * 2, 60, 1);
    }
}

/**
 * @brief Display Hello World message
 */
static void oled_hello_world(void)
{
    ESP_LOGI(TAG, "Displaying Hello World...");
    
    oled_clear();
    
    oled_set_cursor(0, 0);
    oled_write_string("Hello");
    
    oled_set_cursor(0, 2);
    oled_write_string("World!");
    
    oled_set_cursor(0, 4);
    oled_write_string("ESP32 OLED");
}

/**
 * @brief Main application task
 */
static void app_main_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting OLED test application");
    
    // Initialize I2C
    if (i2c_master_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C initialization failed");
        vTaskDelete(NULL);
        return;
    }
    
    // Initialize OLED
    if (oled_init() != ESP_OK) {
        ESP_LOGE(TAG, "OLED initialization failed");
        vTaskDelete(NULL);
        return;
    }
    
    // Clear display
    oled_clear();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Display Hello World
    while (1) {
        // Display Hello World message
        oled_hello_world();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        
        ESP_LOGI(TAG, "Hello World displayed, refreshing...");
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32 OLED Test Application Starting");
    
    // Create main task
    xTaskCreate(app_main_task, "app_main_task", 4096, NULL, 5, NULL);
}
