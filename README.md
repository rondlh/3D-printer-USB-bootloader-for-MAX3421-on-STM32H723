This is a **STM32CubeIDE** project (V1.19.0) that creates a bootloader firmware for the **Mellow Fly D8 STM32H723** (https://mellow.klipper.cn/en/docs/ProductDoc/MainBoard/fly-d/fly-d8-h723/).

Note that there are 2 version of this motherboard, a STM32F407 and a STM32H723 version. Here I use the STM32H723 version.
The firmware useses TinyUSB 0.18 (https://github.com/hathach/tinyusb) and FATFS R0.16 (https://elm-chan.org/fsw/ff/archives.html). CubeIDE also has its own FAT file system stack, which is an outdated version from 2017 (version R0.12c), here R0.16 from 2025 is used. Practically this shouldn't matter I guess, but I prefer the flexibility to have full control over the used FAT file system.

The Mellow Fly D8 motherboard does not have a SD card slot that can be used to flash the firmware as it is targeted to run Klipper. It comes installed with the Klipper Catapult bootloader so Klipper can flash it. I run the Marlin 3D printer firmware (https://github.com/MarlinFirmware/Marlin) on it, so I need a way to flash and update the firmware.
The firmware can be flashed via the onboard USB-C port through DFU. To do this put the board in DFU mode by holding the onboard boot0 (BT0) button while clicking the reset button.
Then use DFU host software to flash the firmware, use DFU-utils (0.11), Visual Studio Code (also uses DFU-utils), or STM32 Cube Programmer to flash the firmware. 
Note that sometimes a virus scanner can cause issues or delays in this process, I usually suspend Avira for a bit when doing this.
For DFU-utils use this command:

`dfu-util.exe -a 0 -s 0x08000000:leave -D H723_MAX3421.bin -d 0483:df11`


Flashing via a computer is not always convenient, so I checked if I can flash the firmware by adding a SD card or even better, flash it via a USB flash drive.
With a suitable SD card adapter and the right firmware flashing via SD card works fine, but that's another project.

I check if the firmware can be flashed via USB, unfortunately none of the 3 onboard USB ports are suitable to read USB flash drives. 
An alternative way is to use a MAX3421 adapter, which connects to the MCU via SPI and can read a USB flash drive. The resulting project is published here.
This project was made for a STM32H723, but can easily be ported to other STM32 hardware.
I added a AMS1117 3.3V power regulator to the MAX3421 board to convert the motherboard 5V to 3.3V, but I still want the USB port to have 5V, so some small modifications are required.
One trace needs to be cut, and a few power routing wire are needed. Only revision 3 of the MAX3421 is supported, which is already very old so anything you buy should be revision 3.
The EXP2 on the D8 motherboard doesn't have 5V, but there is an unconnected pin that can be used for 5V, a small patchwire solves this issue.

**The bootloader**

The bootloader published here is a bit smarter than the average 3D printer bootloader.
It will compare the motherboard flash contents to the provided **firmware.bin** file before flashing, and only flash the file if there is a difference.
It can also verify if the file was flashed correctly. Most bootloaders are silent, but this bootloader gives some feedback of its progress via the serial port. I can see this feedback on the 3D printer display, or via the connected ESP3D V3.0. Matching the serial baud rates will be required to achieve this.
The bootloader will check if the new firmware can fit into the available flash space, and only erases the part of the flash memory that the new firmware will occupy.

An extra feature is that the bootloader can put the board into DFU mode by executing a quick double reset, this helps if you do not have easy access to the onboard buttons. Obviously it requires to wire up an external reset button. Catapult gave me the idea to add this feature.

Flashing via USB is extremely fast, much faster than when using a SD card, but at start up you lose a bit of time (especially when no USB flash key is present) because the system has to wait until the MAX3421 reports the flash drive and file system or that no drive is available.

USB flash drive compatibility seems quite good, but some old (4GB) drives are not compatible. Most flash drives worked fine, including large drives, just format them in **FAT or exFAT** and write the **firmware.bin** file into it and eject the drive from the system safely. If it doesn't work then try another flash drive. The firmware file will be renamed to **firmware.cur** after a successful flash.

After flashing the bootloader I like to make the boot sector read only with STM32CubeProgrammer, so I cannot accidentally overwrite the boot sector. Just uncheck the WRPS0 checkbox in the "OB" (Optional bytes) -> "Write Protection" tab and press Apply. This can always be undone later if needed.
