/*
 * circ_rx_uart_dma.c
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

#include "circ_rx_uart_dma.h"
#include "l4xx_usart_rxevent.h"

#include "hand_side.h"

static struct hand_side *pCircRxForUARTDMA = NULL;

CircRxHandle_t xCircRxForUARTDMA(UARTHandle_t xUART)
{
	return right_for_left_hand_side(&pCircRxForUARTDMA, xUART);
}

UARTHandle_t xUARTDMAForCircRx(CircRxHandle_t xCircRx)
{
	return left_for_right_hand_side(&pCircRxForUARTDMA, xCircRx);
}

static portTASK_FUNCTION(prvCircRxUARTDMATask, pvParameters)
{
	CircRxHandle_t xCircRx = pvParameters;
	UARTHandle_t xUART = xUARTDMAForCircRx(xCircRx);
	uint8_t ucData[circrx_uartdmaBUFFER_LENGTH_BYTES];
	while (HAL_UARTEx_ReceiveToIdle_DMA(xUART, ucData, sizeof(ucData)) != HAL_OK)
		vTaskDelay(circrx_uartdmaDELAY_TICKS);
	vCircRx(xCircRx, ucData, sizeof(ucData));
}

static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer)
{
	CircRxHandle_t xCircRx = xCircRxForUARTDMA(xUART);
	if (xCircRx) xTaskNotifyFromISR(xCircRxTaskHandle(xCircRx), usXfer, eSetValueWithOverwrite, NULL);
}

CircRxHandle_t xCircRxUARTDMACreate(UARTHandle_t xUART, void *pvSender, TxHandler_t xHandler)
{
	CircRxHandle_t xCircRx = xCircRxCreate(pvSender, xHandler);
	push_hand_side(&pCircRxForUARTDMA, xUART, xCircRx);
	TaskHandle_t xTask;
	xTaskCreate(prvCircRxUARTDMATask, "circrxUARTDMA", circrx_uartdmaSTACK_DEPTH, xCircRx, circrx_uartdmaPRIORITY, &xTask);
	configASSERT(xTask != NULL);
	vCircRxTaskHandle(xCircRx, xTask);
	vUSARTRegisterRxEvent(xUART, prvRxEvent);
	return xCircRx;
}
