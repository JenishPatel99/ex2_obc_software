/*
 * Copyright (C) 2022  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/**
 * @file northern_voices_games.c
 * @author Thomas Ganley
 * @date 2022-06-12
 */

#include "northern_voices_games.h"
#include <redposix.h>

static void northern_voices_task(void *pvParameters);

static bool northern_voices_active = false;

NS_return NVM_start_northern_voices(char *filename) {
    if (northern_voices_active == true) {
        sys_log(NOTICE, "Tried to start northern voices when it was already active");
        return NS_FAIL;
    }

    if (xTaskCreate(northern_voices_task, "NVM", NORTHERN_VOICES_STACK_SIZE, (void *)filename,
                    NORTHERN_VOICES_PRIORITY, NULL) != pdPASS) {
        return NS_FAIL;
    } else {
        northern_voices_active = true;
    }
    return NS_OK;
}

void NVM_stop_northern_voices(void) { northern_voices_active = false; }

bool NVM_northern_voices_status(void) { return northern_voices_active; }

static void northern_voices_task(void *pvParameters) {
    char *filename = (char *)pvParameters;
    uint8_t directory_attempts = 3;
    int32_t iErr;

    // Change into northern spirit directory
    while(directory_attempts--){
        iErr = red_chdir("VOL0:/northern_spirit");
        if(iErr == -1){
            sys_log(ERROR, "Error %d trying to enter northern spirit directory\r\n", red_errno);
            if(!directory_attempts){
                northern_voices_active = false;
                vTaskDelete(NULL);
            }

            // Retry in 10 seconds
            vTaskDelay(10 * ONE_SECOND);
            continue;
        }
        break;
    }

    // Open file
    iErr = red_open(filename, RED_O_RDONLY);
    if (iErr == -1) {
        sys_log(ERROR, "Error %d trying to open file %s for northern voices", red_errno, filename);
        northern_voices_active = false;
        vTaskDelete(NULL);
    }

    // Main loop for the northern voices mission
    while (northern_voices_active) {

        // Check power state and discontinue if there are insufficient resources to continue this use of the UHF

        // TODO: Continuously read from file, encode (CODEC) and transmit via UHF PIPE directly

    }
    vTaskDelete(NULL);
}
