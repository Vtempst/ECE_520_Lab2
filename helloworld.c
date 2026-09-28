/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

#define LED_DELAY (100000000)

int main()
{
    XGpio led_gpio;
    XGpio rgb_gpio;
    XGpio switch_gpio;

    int status;
    int switches;
    int binary_out = 0;
    int ring_out = 1;

    volatile int delay;

	init_platform();

	status = XGpio_Initialize(&led_gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
	status = XGpio_Initialize(&rgb_gpio, XPAR_AXI_GPIO_1_DEVICE_ID);
	status = XGpio_Initialize(&switch_gpio, XPAR_AXI_GPIO_2_DEVICE_ID);

	XGpio_SetDataDirection(&led_gpio, 1 , 0x0);
	XGpio_SetDataDirection(&rgb_gpio, 1 , 0x0);
	XGpio_SetDataDirection(&switch_gpio, 1 , 0xF);

    while(1)
	{
    	switches = XGpio_DiscreteRead(&switch_gpio, 1);

    	if (switches == 0x1) // switches = 0001
		{
        	XGpio_DiscreteWrite(&rgb_gpio, 1, 0x4); // 001 rgb sets red
        	XGpio_DiscreteWrite(&led_gpio, 1, 0x1); // 0001 led is on
		}
    	else if (switches == 0x2) // switches = 0010
		{
        	XGpio_DiscreteWrite(&rgb_gpio, 1, 0x2); // 010 rgb sets green
        	XGpio_DiscreteWrite(&led_gpio, 1, 0x2); // 0010 led is on
		}
    	else if (switches == 0x4) // switches = 0100
		{
    		XGpio_DiscreteWrite(&rgb_gpio, 1, 0x1); // 100 rgb sets blue
        	XGpio_DiscreteWrite(&led_gpio, 1, 0x4); // 0100 led is on
		}
    	else if (switches == 0x8) // switches = 1000
		{
    		XGpio_DiscreteWrite(&rgb_gpio, 1, 0x7); // 111 rgb sets blue
        	XGpio_DiscreteWrite(&led_gpio, 1, 0x8); // 1000 led is on
		}
    	else if (switches == 0xC) // switches = 1100 Binary Counter
		{
			XGpio_DiscreteWrite(&rgb_gpio, 1, 0x0); // 000 rgb is off
			XGpio_DiscreteWrite(&led_gpio, 1, binary_out);
			for (delay = 0; delay < LED_DELAY; delay++);
			if (binary_out == 0xF)
			{
				binary_out = 0x0;
			}
			else binary_out++;
		}
    	else if (switches == 0x3) // switches = 0011 Ring Counter
    	{
			XGpio_DiscreteWrite(&rgb_gpio, 1, 0x0); // 000 rgb is off
			XGpio_DiscreteWrite(&led_gpio, 1, ring_out);
			for (delay = 0; delay < LED_DELAY; delay++);
			if (ring_out == 0x8) // 1000
    		{
				ring_out = 0x1; // 0001
    		}
			else
			{
				ring_out = ring_out << 1;
			}
    	}
    	else
    	{
    		XGpio_DiscreteWrite(&rgb_gpio, 1, 0x0); // rgb is off
    		XGpio_DiscreteWrite(&led_gpio, 1, 0x0); // led is off
    	}
	}
    return 0;
}
