/***************************************************************
 * app_config.h
 * Application Configuration for ESP32 OLED Display System
 *
 * This header defines application-level configuration constants,
 * task parameters, and system settings for the ESP32 OLED display
 * project. It centralizes configuration management for the
 * "Hello World! ESP32 OLED" display application.
 *
 * Configuration Areas:
 * - FreeRTOS task parameters (stack size, priority, core assignment)
 * - Display timing and update intervals
 * - Application strings and constants
 * - System-wide configuration settings
 *
 * Features:
 * - Centralized configuration management
 * - Task parameter definitions
 * - Display timing configuration
 * - Application string constants
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Include necessary headers */
#include <stdint.h>
#include <stdbool.h>

/* MISRA C Compliance */
#ifdef __cplusplus
extern "C" {
#endif

/* Application Configuration */
#define APP_TASK_STACK_SIZE      4096
#define APP_TASK_PRIORITY        5
#define APP_TASK_CORE_ID         0

/* Display Configuration */
#define DISPLAY_UPDATE_DELAY_MS  1000
#define DISPLAY_CLEAR_DELAY_MS   100

/* String Constants */
#define APP_NAME_STRING         "ESP32_OLED_Display"
#define APP_VERSION_STRING      "1.0.0"
#define HELLO_STRING            "Hello"
#define WORLD_STRING            "World!"
#define ESP32_OLED_STRING       "ESP32 OLED"

/* Function Declarations */

/**
 * @brief Initialize application configuration
 * @return true on success, false on failure
 */
bool app_config_init(void);

/**
 * @brief Get application name
 * @return Pointer to application name string
 */
const char* app_get_name(void);

/**
 * @brief Get application version
 * @return Pointer to application version string
 */
const char* app_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
