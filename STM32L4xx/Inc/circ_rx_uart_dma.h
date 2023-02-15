/*
 * circ_rx_uart_dma.h
 * Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sub-license, and/or sell  copies  of  the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *		The above copyright notice and this permission notice shall be
 *		included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "circ_rx.h"

#include "l4xx_uart.h"

#define circrx_uartdmaBUFFER_LENGTH_BYTES 32U

#define circrx_uartdmaDELAY_TICKS 1U

#define circrx_uartdmaPRIORITY 16U

CircRxHandle_t xCircRxForUARTDMA(UARTHandle_t xUART);

UARTHandle_t xUARTDMAForCircRx(CircRxHandle_t xCircRx);

/*!
 * \brief Creates a new circular receiver.
 *
 * Use as follows where `handler` identifies a relay transmission function, i.e.
 * somewhere to pass bytes on successful receipt. The `NULL` passes to the
 * function's first parameter, and ignored in this excerpt.
 * \code{.c}
 * xCircRxUARTDMACreate(&hlpuart1, NULL, handler);
 * \encode
 * Uses dynamic memory for space allocation. Expression `&lpuart1` identifies
 * the low-power UART channel to wire up for receiving using DMA. The channel
 * needs a circular DMA channel.
 */
CircRxHandle_t xCircRxUARTDMACreate(UARTHandle_t xUART, void *pvSender, TxHandler_t Handler);
