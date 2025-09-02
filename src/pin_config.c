/***************************************************************
 * pin_config.c
 * ESP32 Pin Configuration Implementation
 *
 * Project: VDU_ESP32 (Vehicle Display Unit)
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

/* Include necessary headers */
#include "pin_config.h"
#include "esp_log.h"
#include "driver/gpio.h"

/* Private constants */
static const char* TAG = "PIN_CONFIG";

/* Function implementations */

esp_err_t pin_config_init(void)
{
    esp_err_t ret = ESP_OK;
    
    ret |= pin_config_gpio();
    ret |= pin_config_i2c();
    
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Pin configuration initialized successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Pin configuration failed");
    }
    
    return ret;
}

esp_err_t pin_config_i2c(void)
{
    esp_err_t ret = ESP_OK;
    
    /* Configure I2C SDA pin */
    gpio_config_t sda_config = {
        .pin_bit_mask = (1ULL << I2C_MASTER_SDA_IO),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret |= gpio_config(&sda_config);
    
    /* Configure I2C SCL pin */
    gpio_config_t scl_config = {
        .pin_bit_mask = (1ULL << I2C_MASTER_SCL_IO),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret |= gpio_config(&scl_config);
    
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "I2C pins configured successfully");
    }
    else
    {
        ESP_LOGE(TAG, "I2C pin configuration failed");
    }
    
    return ret;
}

esp_err_t pin_config_gpio(void)
{
    esp_err_t ret = ESP_OK;
    
    /* Configure built-in LED */
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << GPIO_LED_BUILTIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret |= gpio_config(&led_config);
    
    /* Configure button pins */
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << GPIO_BUTTON_1) | (1ULL << GPIO_BUTTON_2),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret |= gpio_config(&button_config);
    
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "GPIO pins configured successfully");
    }
    else
    {
        ESP_LOGE(TAG, "GPIO pin configuration failed");
    }
    
    return ret;
}
