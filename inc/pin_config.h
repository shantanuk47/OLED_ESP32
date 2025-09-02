/***************************************************************
 * pin_config.h
 * ESP32 Pin Configuration Definitions
 *
 * Project: VDU_ESP32 (Vehicle Display Unit)
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

/* Include necessary headers */
#include "driver/gpio.h"
#include "driver/i2c.h"

/* MISRA C Compliance */
#ifdef __cplusplus
extern "C" {
#endif

/* I2C Pin Configuration */
#define I2C_MASTER_SCL_IO        22
#define I2C_MASTER_SDA_IO        21
#define I2C_MASTER_NUM           I2C_NUM_0
#define I2C_MASTER_FREQ_HZ       400000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS    1000

/* GPIO Pin Definitions */
#define GPIO_LED_BUILTIN         2
#define GPIO_BUTTON_1            0
#define GPIO_BUTTON_2            35

/* SPI Pin Configuration (for future use) */
#define SPI_MOSI_PIN             23
#define SPI_MISO_PIN             19
#define SPI_CLK_PIN              18
#define SPI_CS_PIN               5

/* UART Pin Configuration (for future use) */
#define UART_TX_PIN              1
#define UART_RX_PIN              3

/* Function Declarations */

/**
 * @brief Initialize GPIO pins
 * @return ESP_OK on success, error code on failure
 */
esp_err_t pin_config_init(void);

/**
 * @brief Configure I2C pins
 * @return ESP_OK on success, error code on failure
 */
esp_err_t pin_config_i2c(void);

/**
 * @brief Configure GPIO pins for general use
 * @return ESP_OK on success, error code on failure
 */
esp_err_t pin_config_gpio(void);

#ifdef __cplusplus
}
#endif

#endif /* PIN_CONFIG_H */
