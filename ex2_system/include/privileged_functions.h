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

/*
 * privileged_functions.h
 *
 *  Created on: Jun. 14, 2021
 *      Author: Robert Taylor
 */

#ifndef PRIVILEGED_FUNCTIONS_H_
#define PRIVILEGED_FUNCTIONS_H_

/*
 * This file works as a wrapper to allow including privileged functions in multiple files
 * Since privileged functions must be implemented in the header per FreeRTOS rules it would
 * fail to compile if they were included in more than one place.
 */
#define raise_privilege raise_privilege_
#define reset_privilege reset_privilege_

#endif /* INCLUDE_PRIVILEGED_FUNCTIONS_H_ */
