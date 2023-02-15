#pragma once

#include "l4xx_uart.h"

/*!
 * \brief Function that handles a generic UART event.
 */
typedef void (*UARTHandler_t)(UARTHandle_t xUART);

/*!
 * \brief Function that handles a UART receiver event.
 */
typedef void (*UARTRxEventHandler_t)(UARTHandle_t xUART, uint16_t usXfer);
