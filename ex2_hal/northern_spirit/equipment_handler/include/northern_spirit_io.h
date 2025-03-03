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
 * @file northern_spirit_io.c
 * @author Thomas Ganley
 * @date 2021-10-26
 */
#ifndef NORTHERN_SPIRIT_IO_H
#define NORTHERN_SPIRIT_IO_H

#include "northern_spirit_handler.h"
#include "FreeRTOS.h"

#define NS_SEMAPHORE_TIMEOUT_MS pdMS_TO_TICKS(500)
#define NS_UART_TIMEOUT_MS pdMS_TO_TICKS(1000)
#define NS_UART_LONG_TIMEOUT_MS pdMS_TO_TICKS(5000)

NS_return init_ns_io();

NS_return NS_sendAndReceive(uint8_t *command, uint32_t command_length, uint8_t *answer, uint8_t answer_length);
NS_return NS_sendOnly(uint8_t *command, uint32_t command_length);
NS_return NS_expectResponse(uint8_t *response, uint8_t length);
void NS_resetQueue(void);

#endif // NORTHERN_SPIRIT_IO_H
