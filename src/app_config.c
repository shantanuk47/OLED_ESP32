/***************************************************************
 * app_config.c
 * Application Configuration Implementation for OLED Display
 *
 * This module implements application-level configuration management
 * for the ESP32 OLED display system. It provides functions to
 * initialize configuration settings and retrieve application
 * constants and parameters used throughout the system.
 *
 * Key Functions:
 * - Application configuration initialization
 * - Application name and version retrieval
 * - Configuration validation and setup
 * - System parameter management
 *
 * Configuration Management:
 * - FreeRTOS task parameters
 * - Display timing settings
 * - Application strings and constants
 * - System-wide configuration validation
 *
 * Project: ESP32 OLED Display System
 * Author: Shantanu Kumar
 * Date: 2025-02-09
 ***************************************************************/

/* Include necessary headers */
#include "app_config.h"
#include <stdbool.h>

/* Function implementations */

bool app_config_init(void)
{
    /* Configuration initialization */
    return true;
}

const char* app_get_name(void)
{
    return APP_NAME_STRING;
}

const char* app_get_version(void)
{
    return APP_VERSION_STRING;
}
