/***************************************************************
 * app_config.c
 * Application Configuration Implementation
 *
 * Project: VDU_ESP32 (Vehicle Display Unit)
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
