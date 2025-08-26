/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------
// Common Configuration
//--------------------------------------------------------------------

// USB HOST
#define CFG_TUSB_MCU       	   OPT_MCU_STM32H7 // MCU family, important!!!
#define CFG_TUSB_OS        	   OPT_OS_NONE     // Operating system used (NONE/RTOS)
#define BOARD_TUH_RHPORT   	   0 // Root hub port number
#define CFG_TUH_MAX3421 	   1 // Enable MAX3421
#define CFG_TUH_ENABLED    	   1 // Enable Host stack
#define BOARD_TUH_MAX_SPEED    (OPT_MODE_FULL_SPEED)
#define CFG_TUSB_RHPORT1_MODE  (OPT_MODE_HOST | OPT_MODE_FULL_SPEED)
#define CFG_TUH_MAX_SPEED      (OPT_MODE_FULL_SPEED) // Max speed the controller can use with internal PHY

#ifndef CFG_TUSB_DEBUG
  #define CFG_TUSB_DEBUG        0
#endif

/* USB DMA on some MCUs can only access a specific SRAM region with restriction on alignment.
 * Tinyusb use follows macros to declare transferring memory so that they can be put
 * into those specific section.
 * e.g
 * - CFG_TUSB_MEM SECTION : __attribute__ (( section(".usb_ram") ))
 * - CFG_TUSB_MEM_ALIGN   : __attribute__ ((aligned(4)))
 */
#ifndef CFG_TUH_MEM_SECTION
  #define CFG_TUH_MEM_SECTION
#endif

#ifndef CFG_TUH_MEM_ALIGN
  #define CFG_TUH_MEM_ALIGN     __attribute__ ((aligned(4)))
#endif

//------------------------- Board Specific --------------------------

// RHPort max operational speed can defined by board.mk
#ifndef BOARD_TUH_MAX_SPEED
  #define BOARD_TUH_MAX_SPEED   OPT_MODE_DEFAULT_SPEED
#endif

//--------------------------------------------------------------------
// Driver Configuration
//--------------------------------------------------------------------

// Size of buffer to hold descriptors and other data used for enumeration
#define CFG_TUH_ENUMERATION_BUFSIZE 256

#define CFG_TUH_HUB                 0 // number of supported hubs
#define CFG_TUH_MSC                 1
#define CFG_TUH_CDC                 0
#define CFG_TUH_HID                 0 // typical keyboard + mouse device can have 3-4 HID interfaces
#define CFG_TUH_VENDOR              0

// max device support (excluding hub device)
#define CFG_TUH_DEVICE_MAX          1

//------------- MSC -------------//
#define CFG_TUH_MSC_MAXLUN          1

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_CONFIG_H_ */
