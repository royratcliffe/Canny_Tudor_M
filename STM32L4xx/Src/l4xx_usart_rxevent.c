/*
 * \file l4xx_usart_rxevent.c
 * \attention Copyright (c) 2023, Roy Ratcliffe, Northumberland, United Kingdom
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

#include "l4xx_usart_rxevent.h"
#include "l4xx_usart.h"

#include "hand_side.h"

static struct hand_side *pRxEventForUART[l4xx_usartMAX_INSTANCES];

/*
 * Left-hand side matches the UART handle, right-hand side matches the handler.
 * Match the incoming UART handle against the left-hand side and invoke the
 * right-hand side handler.
 */
#if USE_HAL_UART_REGISTER_CALLBACKS
static void prvRxEvent(UARTHandle_t xUART, uint16_t usXfer)
#else
void HAL_UARTEx_RxEventCallback(UARTHandle_t xUART, uint16_t usXfer)
#endif
{
	struct hand_side **ppRxEventForUART = pRxEventForUART + ucCardinalForUSART(xUART->Instance);
	UARTRxEventHandler_t xHandler = right_for_left_hand_side(ppRxEventForUART, xUART);
	if (xHandler != NULL) xHandler(xUART, usXfer);
}

void vUSARTRegisterRxEvent(UARTHandle_t xUART, UARTRxEventHandler_t xHandler)
{
	struct hand_side **ppRxEventForUART = pRxEventForUART + ucCardinalForUSART(xUART->Instance);
#if USE_HAL_UART_REGISTER_CALLBACKS
	if (*ppRxEventForUART == NULL) HAL_UART_RegisterRxEventCallback(xUART, prvRxEvent);
#endif
	(void)pop_left_hand_side(ppRxEventForUART, xUART);
	push_hand_side(ppRxEventForUART, xUART, xHandler);
}
