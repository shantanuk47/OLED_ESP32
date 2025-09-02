/***************************************************************
 * app_main.h
 * Main Application Header for ESP32 OLED Display System
 *
 * This header defines the main application interface and entry points
 * for the ESP32 OLED display system. It provides function declarations
 * for the main application logic, task management, and display
 * demonstration functions.
 *
 * Main Components:
 * - Application entry point and initialization
 * - Main application task implementation
 * - Hello World display demonstration
 * - Test pattern display functions
 *
 * Features:
 * - FreeRTOS task management
 * - OLED display demonstration
 * - Error handling and logging
 * - Application lifecycle management
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

#ifndef APP_MAIN_H
#define APP_MAIN_H

/* Include necessary headers */
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* MISRA C Compliance */
#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */

/**
 * @brief Main application entry point
 * @return ESP_OK on success, error code on failure
 */
esp_err_t app_main(void);

/**
 * @brief Main application task
 * @param pvParameters Task parameters (unused)
 */
void app_main_task(void *pvParameters);

/**
 * @brief Display Hello World message
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_hello_world(void);

/**
 * @brief Test pattern display function
 * @return ESP_OK on success, error code on failure
 */
esp_err_t oled_test_pattern(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_MAIN_H */
