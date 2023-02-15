/*
 * l4xx_uart.h
 *
 *  Created on: Feb 14, 2023
 *      Author: RoyRatcliffe
 */

#pragma once

#include "l4xx_mcu.h"
#include "stm32l4xx_hal_uart.h"

/*!
 * \brief UART handle
 *
 * Wraps the pointer to the UART structure as the handle type. Makes the pointer
 * become the handle, rather than a pointer to a handle. The HAL interface calls
 * the structure a handle for which it accepts a pointer reference.
 */
typedef UART_HandleTypeDef *UARTHandle_t;
