## The FreeRTOS Kernel Library

__Note: modifiying these files may break PROS or make PROS unstable__

_This file serves as an introduction to how PROS uses FreeRTOS if you're just_
_getting started with development of the PROS kernel. It also contains some_
_notes from configuring FreeRTOS to work with the V5._

PROS 3 uses FreeRTOS 9.0.0 to implement scheduling, queues, and lists - the 
barebones of an operating system. In some respects, FreeRTOS could be thought of
as a C library that we have ported to the V5. It of course does more significant
work that a typical application library.
Since computer architectures are different and FreeRTOS aims to work with many
embedded architectures, some work must be done to let FreeRTOS work with the 
hardware.

FreeRTOS provides many demo projects and the developers recommend modifying one 
of the projects to suit the needs of the project. Since the V5 is Zynq-7000 board,
we're modifying the Cortex\_A9\_Zynq\_ZC702 demo.

You should read some of FreeRTOS's documentation and/or take an equivalent of 
Purdue's CS250 (Computer Architecture) and CS354 (Operating Systems) for 
requisite background knowledge. For more info about the Zynq/A9 port, see 
FreeRTOS's documentation on [Zynq](http://www.freertos.org/RTOS-Xilinx-Zynq.html) 
and the [A9](http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html).

## Modifications to FreeRTOS
We've somewhat significantly refactored the FreeRTOS kernel so that any FreeRTOS
functions used within PROS align with the PROS coding style. See refactor.tsv in
this directory.

We've removed the use of `int32_t` and `uint32_t` from any public facing
API since we felt it unnecessary. Additionally, all variables pertaining to ticks
on the PROS internal or public facing APIs have become millisecond precision, and 
the conversion done immediately upon entering the functiion.

