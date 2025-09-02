/***************************************************************
 * app_main.c
 * Main Application Implementation for ESP32 OLED Display System
 *
 * This module implements the main application logic for the ESP32
 * OLED display system. It handles system initialization, task
 * creation, and the main application loop that demonstrates
 * "Hello World! ESP32 OLED" display functionality.
 *
 * Application Flow:
 * - System initialization (pins, I2C, OLED)
 * - FreeRTOS task creation and management
 * - Continuous Hello World display demonstration
 * - Error handling and logging throughout
 *
 * Key Functions:
 * - Application entry point and initialization
 * - Main application task with display loop
 * - Hello World display demonstration
 * - Test pattern display functions
 *
 * Display Demonstration:
 * Shows "Hello", "World!", and "ESP32 OLED" on separate lines
 * with continuous clearing and redisplay every second.
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

/* Include necessary headers */
#include "app_main.h"
#include "app_config.h"
#include "oled_driver.h"
#include "pin_config.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Private constants */
static const char* TAG = "APP_MAIN";

/* Function implementations */

esp_err_t app_main(void)
{
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Starting ESP32 OLED Display System");
    ESP_LOGI(TAG, "Version: %s", app_get_version());
    
    /* Initialize application configuration */
    if (!app_config_init())
    {
        ESP_LOGE(TAG, "Application configuration initialization failed");
        return ESP_FAIL;
    }
    
    /* Initialize pin configuration */
    ret = pin_config_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Pin configuration failed: %s", esp_err_to_name(ret));
        return ret;
    }
    
    /* Initialize I2C master */
    ret = i2c_master_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C master initialization failed: %s", esp_err_to_name(ret));
        return ret;
    }
    
    /* Initialize OLED display */
    ret = oled_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "OLED initialization failed: %s", esp_err_to_name(ret));
        return ret;
    }
    
    /* Create main application task */
    BaseType_t task_ret = xTaskCreatePinnedToCore(
        app_main_task,
        "app_main_task",
        APP_TASK_STACK_SIZE,
        NULL,
        APP_TASK_PRIORITY,
        NULL,
        APP_TASK_CORE_ID
    );
    
    if (task_ret != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create main application task");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Application initialized successfully");
    return ESP_OK;
}

void app_main_task(void *pvParameters)
{
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Main application task started");
    
    /* Display Hello World message */
    ret = oled_hello_world();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to display Hello World: %s", esp_err_to_name(ret));
    }
    
    /* Main application loop */
    while (true)
    {
        /* Wait for display update interval */
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_DELAY_MS));
        
        /* Clear display */
        ret = oled_clear();
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to clear display: %s", esp_err_to_name(ret));
        }
        
        /* Wait before displaying again */
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_CLEAR_DELAY_MS));
        
        /* Display Hello World again */
        ret = oled_hello_world();
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to display Hello World: %s", esp_err_to_name(ret));
        }
    }
}

esp_err_t oled_hello_world(void)
{
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Displaying Hello World...");
    
    ret |= oled_clear();
    if (ret != ESP_OK)
    {
        return ret;
    }
    
    ret |= oled_set_cursor(0, 0);
    ret |= oled_write_string(HELLO_STRING);
    
    ret |= oled_set_cursor(0, 2);
    ret |= oled_write_string(WORLD_STRING);
    
    ret |= oled_set_cursor(0, 4);
    ret |= oled_write_string(ESP32_OLED_STRING);
    
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Hello World displayed successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to display Hello World");
    }
    
    return ret;
}

esp_err_t oled_test_pattern(void)
{
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Displaying test pattern...");
    
    ret |= oled_clear();
    if (ret != ESP_OK)
    {
        return ret;
    }
    
    /* Draw test pattern - border rectangle */
    ret |= oled_draw_rect(0, 0, OLED_WIDTH, OLED_HEIGHT);
    
    /* Draw diagonal lines */
    ret |= oled_draw_line(0, 0, OLED_WIDTH - 1, OLED_HEIGHT - 1);
    ret |= oled_draw_line(OLED_WIDTH - 1, 0, 0, OLED_HEIGHT - 1);
    
    /* Draw center cross */
    ret |= oled_draw_line(OLED_WIDTH / 2, 0, OLED_WIDTH / 2, OLED_HEIGHT - 1);
    ret |= oled_draw_line(0, OLED_HEIGHT / 2, OLED_WIDTH - 1, OLED_HEIGHT / 2);
    
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Test pattern displayed successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to display test pattern");
    }
    
    return ret;
}
