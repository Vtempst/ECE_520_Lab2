# ECE_520_Lab2

The source file that includes all the code necessary for the lab is in the file path: axi_gpio_vitis/axi_gpio_app/src/helloworld.c

## Overview

The lab introduces the Zynq SoC design flow using the Zybo Z7 board, Vivado, Vitis, and AXI GPIO peripherals.

The lab is split into two parts:
1. Instantiating AXI GPIO peripherals in the Vivado block design.
2. Writing a C program in Vitis that controls the hardware in the FPGA portion of the Zynq chip.

The key takeaway of the lab is that the Zybo Z7 includes both a Processing System (PS) and Programmable Logic (PL). The ARM processor is in the PS, while the FPGA logic is placed in the PL. The processor communicates with the GPIO blocks through the AXI bus. AXI is the bus protocol that is used to connect hardware components inside the FPGA.

## Design Summary

The design began by creating a Block Design in Vivado and adding the ZYNQ7 processing system, which instantiates the Zynq PS block in the block design. Then, AXI GPIO IP is instantiated and defined in the block design. Vivado automatically configures the connections between the AXI GPIO peripherals and the processor. After validating the design, the lab required creating an HDL wrapper, generating the bitstream, and exporting the hardware as an XSA file.

The next part of the lab was to create a platform project from the XSA file as well as to create an application project with the hello world template. The hello world template was modified to control the AXI GPIO.

The design includes three AXI GPIO peripherals in the Vivado block design. AXI GPIO #1 is connected to the Zybo Z7 LEDs (LED0 to LED3). AXI GPIO #2 is connected to the Zybo Z7 RGB led (Bit 0: Red, Bit 1: Green, Bit 2: Blue). Lastly, AXI GPIO #3 is connected to Zybo Z7 switches (SW0 to SW3).

The software began by initializing the AXI GPIO peripherals using their respective device IDs.

```
XGpio_Initialize(&led_gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
XGpio_Initialize(&rgb_gpio, XPAR_AXI_GPIO_1_DEVICE_ID);
XGpio_Initialize(&switch_gpio, XPAR_AXI_GPIO_2_DEVICE_ID);
```

The next step was to configure the LEDs and the RGB LED as outputs. The switches were configured as switches.

```
XGpio_SetDataDirection(&led_gpio, 1 , 0x0);
XGpio_SetDataDirection(&rgb_gpio, 1 , 0x0);
XGpio_SetDataDirection(&switch_gpio, 1 , 0xF);
```
The following commands were used to set which LEDs were active and which color to set the RGB LED by changing X to the required bits:
```
XGpio_DiscreteWrite(&rgb_gpio, 1, X);
XGpio_DiscreteWrite(&led_gpio, 1, X);
```
The RGB LED is driven by a 3-bit value in which each bit controls one channel: 
- Bit 0 - Red 
- Bit 1 - Green
- Bit 2 - Blue

The software program was designed to continuously read the switches and display the required output. The behavior of the LEDs and RGB LED for each switch value was based on the lab document provided. 

The program implemented two counter modes:
- When SW0 and SW1 are both on while SW2 and SW3 are both off, the LEDs display a 4-bit binary counter that counts up from 0x0 to 0xF and wraps back to 0x0.
- When SW2 and SW3 are both on while SW0 and SW1 are both off, the LEDs display a 4-bit ring counter that starts with only LED0 on. The active LED shifts one position to the left and wraps back around to LED0.

## Verification

The design did not require a test bench to simulate behavior. The design was verified exclusively through visual observation. The design went through many iterations and the program was modified to fit the required behavior of the outputs. The behavior was visually confirmed for every switch condition.

## Known Issues or Limitations:

A limitation was that the RGB LED did not follow the expected behavior. When the RGB LED was set to 0x1,it displayed blue instead of red. When RGB LED was set to 0x4, it displayed red instead of blue. I switched the bit values for the two cases to fit the needs of the lab.

## References

The xgpio.c source file and the xparameters.h header file provided by Vitis were referenced.