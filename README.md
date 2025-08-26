This is a STM32 CubeIDE project (V1.19.0) that creates a bootloader firmware for the Mellow Fly D8 STM32H723 (https://mellow.klipper.cn/en/docs/ProductDoc/MainBoard/fly-d/fly-d8-h723/).
Note that there are 2 version of this motherboard, a STM32F407 and a STM32H723 version. Here I use the STM32H723 version.
The firmware useses TinyUSB 0.18 (https://github.com/hathach/tinyusb) and FATFS R0.16 (https://elm-chan.org/fsw/ff/archives.html).
The Mellow Fly D8 motherboard does not have a SD card slot that can be used to flash the firmware as it is targeted to run Klipper. It comes installed with the Klipper Catapult bootloader so Klipper can flash it.
I run the Marlin 3D printer firmware (https://github.com/MarlinFirmware/Marlin) on it, so I need a way to flash and update the firmware.
The firmware can be flashed via the onboard USB-C port through DFU. To do this put the board in DFU mode by holding the onboard boot0 (BT0) button while clicking the reset button.
Then use DFU host software to flash the firmware, use DFU-utils (0.11), Visual Studio Code (also uses DFU-utils), or STM32 Cube Programmer to flash the firmware.
Flashing via a computer is not always convenient, so I checked if I can flash it by adding a SD card or even better, flash it via a USB flash drive.
With a suitable SD card adapter and the right software flashing via SD card works fine, but that's another project.
I check if the firmware can be flashed via USB, unfortunately none of the onboard USB ports are suitable to read USB flash drives. 
An alternative way is to use a MAX3421 adapter, which connects to the MCU via SPI and can read a USB flash drive. The resulting project is published here.
This project was made for a STM32H723, but can easily be ported to other STM32 hardware.
I added a AMS1117 3.3V power regulator to the MAX3421 board to convert the motherboard 5V to 3.3V, but I still want the USB port to have 5V, so some small modifications are required.
One trace needs to be cut, and a few power routing wire are needed. Only revision 3 of the MAX3421 is supported, which is already very old so anything you buy should be revision 3.
The EXP2 on the D8 motherboard doesn't have 5V, but there is an unconnected pin that can be used for 5V, a small patchwire solves this issue.
