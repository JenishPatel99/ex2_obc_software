/*
 * Copyright (C) 2021  University of Alberta
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
 * @file northern_spirit_handler.h
 * @author Thomas Ganley
 * @date 2021-10-22
 */

#ifndef NORTHERN_SPIRIT_HANDLER_H
#define NORTHERN_SPIRIT_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include "logger.h"

// Lengths of commands being sent
#define NS_STANDARD_CMD_LEN 3
#define NS_SUBCODED_CMD_LEN 6

// Lengths of expected answers
#define NS_STANDARD_ANS_LEN 2

#define NS_FLAG_DATA_LEN 1
#define NS_FILENAME_DATA_LEN 11
#define NS_ENCODED_TELEMETRY_DATA_LEN 64
#define NS_DECODED_TELEMETRY_DATA_LEN 48
#define NS_SWVERSION_DATA_LEN 7

#define NS_IMAGE_COLLECTION_DELAY pdMS_TO_TICKS(10000)
#define NS_TELEMETRY_COLLECTION_DELAY pdMS_TO_TICKS(15000)
#define NS_GETFLAG_DELAY pdMS_TO_TICKS(2000)
#define NS_GETFILENAME_DELAY pdMS_TO_TICKS(2000)
#define NS_CONFIRM_DOWNLINK_DELAY pdMS_TO_TICKS(1000)
#define NS_COMMAND_MUTEX_TIMEOUT pdMS_TO_TICKS(1000)

typedef enum {
    NS_OK,
    NS_FAIL,
    NS_BAD_CMD,
    NS_BAD_ANS,
    NS_HANDLER_BUSY,
    NS_UART_FAIL,
    NS_UART_BUSY,
    NS_MALLOC_FAIL,

    NS_IS_STUBBED = 0
} NS_return;

typedef struct __attribute__((packed)) {
    int16_t temp0;
    int16_t temp1;
    int16_t temp2;
    int16_t temp3;
    int16_t eNIM0;
    int16_t eNIM1;
    int16_t eNIM2;
    int16_t ram_avail;
    int16_t lowest_img_num;
    int16_t first_blank_img_num;
} ns_telemetry;

typedef enum {
    CameraStatus = 'c',
    SDcardStatus = 's',
} NS_flag_subcodes;

NS_return NS_handler_init(void);
NS_return NS_upload_artwork(char *filename);
NS_return NS_capture_image(void);
NS_return NS_confirm_downlink(uint8_t *conf);
NS_return NS_get_heartbeat(uint8_t *heartbeat);
NS_return NS_get_flag(char flag, bool *stat);
NS_return NS_get_filename(char subcode, char *filename);
NS_return NS_get_telemetry(ns_telemetry *telemetry);
NS_return NS_get_software_version(uint8_t *version);

#endif // NORTHERN_SPIRIT_HANDLER_H
