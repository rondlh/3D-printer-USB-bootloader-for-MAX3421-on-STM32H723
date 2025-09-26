/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/*********************************************************************************************
* IRON 2025 smart USB bootloader for Mellow Fly D8 Pro (STM32H723) with MAX3421 at EXP2 port *
*********************************************************************************************/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdarg.h>      // For uart_printf
#include <string.h>      // For memset
#include "tusb.h"        // TinyUSB
#include "tusb_config.h" // TinyUSB configuration
#include "ff.h"          // FatFS
#include "diskio.h"      // DiskIO for FatFS

// Function prototypes of API functions
void tuh_max3421_spi_cs_api(uint8_t rhport, bool active);
uint8_t tuh_max3421_reg_read(uint8_t rhport, uint8_t reg, bool in_isr);
bool tuh_max3421_reg_write(uint8_t rhport, uint8_t reg, uint8_t data, bool in_isr);

// define the buffer to be place in USB/DMA memory with correct alignment/cache line size
CFG_TUH_MEM_SECTION static struct
{
  TUH_EPBUF_TYPE_DEF(scsi_inquiry_resp_t, inquiry);
} scsi_resp;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DEBUG_USART_HANDLE      huart1                                  // The UART to send debug info to
#define SPI_HANDLE              hspi2                                   // SPI device for MAX3421 communication
// NOTE: Select "LOW" for the SPI pins maximum frequency

#define FAST_SPI                                                        // Use a faster/leaner SPI implementation

#define VERBOSE_MODE                                                    // Comment out to disable sending info to USART (250K baud)
#define COMPARE_BEFORE_FLASH                                            // Comment out to not compare the firmware file to the flash contents (faster)
#define FIRMWARE_FILENAME        "firmware.bin"                            // The firmware file to flash
#define FIRMWARE_RENAME            "firmware.cur"                            // Rename the firmware after flashing

// Make sure pins don't interfere with SWD debug pins PA13 and PA14, disable when debugging
#define PROGRESS_LED_PIN        GPIO_PIN_13                                // Progress LED pin, flashes during flash update
#define PROGRESS_LED_PORT        GPIOA                                    // Progress LED port

#define DFU_ON_DOUBLE_RESET                                                // Double reset to start DFU mode, comment out to disable
#define DFU_MAGIC_KEY            0xBA5EBA11                                // Magic key to detect DFU mode
#define DFU_MAGIC_KEY_ADDRESS    RTC->BKP31R                                // Store the magic key at RTC backup register 31

#define FLASHWORD                (FLASH_NB_32BITWORD_IN_FLASHWORD * 4U)    // 32 bytes on STM32H7
#define FILE_BUFFER_SIZE        4096UL                                    // File buffer size, must be dividable by FLASHWORD

#define FLASH_BOOTLOADER_SIZE    (FLASH_SECTOR_SIZE * 1)                    // Bootloader flash size (1 sector)
#define FLASH_USER_START_SECTOR    1U                                        // Bootloader in sector 0, start from sector 1
#define FLASH_USER_START_ADDR    (FLASH_BASE + FLASH_BOOTLOADER_SIZE)    // Should start on a new sector boundary

/* STM32 DFU bootloader addresses
   STM32C0   0x1FFF0000 | STM32F030x8 0x1FFFEC00 | STM32F030xC 0x1FFFD800 | STM32F03xx 0x1FFFEC00
   STM32F05  0x1FFFEC00 | STM32F07    0x1FFFC800 | STM32F09    0x1FFFD800 | STM32F10xx 0x1FFFF000
   STM32F105 0x1FFFB000 | STM32F107   0x1FFFB000 | STM32F10XL  0x1FFFE000 | STM32F2    0x1FFF0000
   STM32F3   0x1FFFD800 | STM32F4     0x1FFF0000 | STM32F7     0x1FF00000 | STM32G0    0x1FFF0000
   STM32G4   0x1FFF0000 | STM32H503   0x0BF87000 | STM32H563   0x0BF97000 | STM32H573  0x0BF97000
   STM32H7x  0x1FF09800 | STM32H7A    0x1FF0A800 | STM32H7B    0x1FF0A000 | STM32L0    0x1FF00000
   STM32L1   0x1FF00000 | STM32L4     0x1FFF0000 | STM32L5     0x0BF90000 | STM32WBA   0x0BF88000
   STM32WBX  0x1FFF0000 | STM32WL     0x1FFF0000 | STM32U5     0x0BF90000 */

#define DFU_BOOTLOADER_ADDRESS    0x1FF09800U    // STM32H7xx address to start the DFU bootloader

/* FATFS ERROR CODES
 0 Succeeded
 1 A hard error occurred in the low level disk I/O layer
 2 Assertion failed
 3 The physical drive cannot work
 4 Could not find the file
 5 Could not find the path
 6 The path name format is invalid
 7 Access denied due to prohibited access or directory full
 8 Access denied due to prohibited access
 9 The file/directory object is invalid
10 The physical drive is write protected
11 The logical drive number is invalid
12 The volume has no work area
13 There is no valid FAT volume
14 The f_mkfs() aborted due to any problem
15 Could not get a grant to access the volume within defined period
16 The operation is rejected according to the file sharing policy
17 LFN working buffer could not be allocated
18 Number of open files > _FS_LOCK
19 Given parameter is invalid */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

FATFS FatFs;                          // FAT File System handle
FIL fwFile;                           // File handle for the firmware file
FRESULT result;                          // File operation result
uint32_t fileSize;                    // Firmware file size in bytes
uint8_t fileBuffer[FILE_BUFFER_SIZE] __attribute__((aligned(4))); // File read buffer
volatile bool _disk_busy = false;     // For FatFs

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Calculate CRC32 of a block of data
#define CRC32_START 0xFFFFFFFF  // Start value for the CRC32 calculation
uint32_t crc32b(uint32_t crc, uint8_t *data, uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        crc = crc ^ data[i];
        for (int j = 8; j; j--)
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }
    return ~crc;
}

#ifdef VERBOSE_MODE // No serial output if not in VERBOSE_MODE

    // Lightweight printf, prints to uart (DEBUG_USART_HANDLE), no floats, no width control
    void uart_printf(const char * fmt, ...)
    {
        va_list va;
        va_start(va, fmt);
        char debug_msg[255]; // Message buffer
        char * buf = debug_msg;
        char space_zero = ' ';
        char c;
        unsigned int num;
        while ((c = *(fmt++)))
        {
            int width = 0;
            if (c == '%')
            {
                int base = 2;
                int s_int = 0;
            MORE_FORMAT:
                c = *(fmt++); // Skip '%', check parameter
                switch (c)
                {
                    case '0':
                        if (width == 0)
                            space_zero = '0';
                    case '1'...'9': // Width indicators
                          width = (width * 10) + c - '0';
                    goto MORE_FORMAT;

                    case '%': // "%%" prints "%"
                         *(buf++) = '%';
                    break;

                    case 'c': // Character
                         *(buf++) = va_arg(va, int);
                    break;

                    case 'd': // Signed integer, base 10
                    case 'i': base = 10;
                        s_int = va_arg(va, int);
                        if (s_int < 0)
                               num = -s_int;
                        else
                              num = s_int;
                      goto ATOI;
                    case 'x':
                    case 'X':      // Hexadecimal, base 16
                        base += 6; // 2 + 6 + 8 is base 16
                    case 'u':      // Unsigned integer, base 10
                        base += 8; // 2 + 8 is base 10
                    case 'b':      // Binary, base 2
                        num = va_arg(va, unsigned int);
                      ATOI:
                        char tmp[32]; // 32bit
                        char *q = tmp;

                        do
                        {
                            int rem = '0' + (num % base);
                            if (rem > '9')
                                  rem += 7; // Map to 'ABCDEF'
                               *(q++) = rem;
                        } while ((num /= base));

                        if (s_int < 0)
                              *(q++) = '-';

                        width -= q - tmp;
                        while (width-- > 0)
                              *(buf++) = space_zero;

                           while (tmp < q) // Reverse data order, "123" --> "321"
                           *(buf++) = *(--q);
                    break;

                    case 's':  // String
                    {
                        const char *p = va_arg(va, const char *);
                        while (*p)
                            *(buf++) = *(p++);
                    }
                }
            }
            else
                *(buf++) = c; // Copy literal characters
        }
        *buf = '\0'; // Terminate string

        va_end(va);

        HAL_UART_Transmit(&DEBUG_USART_HANDLE, (uint8_t *)debug_msg, buf - debug_msg, HAL_MAX_DELAY);

        #ifdef PROGRESS_LED_PIN
            HAL_GPIO_TogglePin(PROGRESS_LED_PORT, PROGRESS_LED_PIN); // Flash LED
        #endif
    }

#else

    #ifdef PROGRESS_LED_PIN
        void uart_printf(const char * fmt, ...) { HAL_GPIO_TogglePin(PROGRESS_LED_PORT, PROGRESS_LED_PIN); }
    #else
        void uart_printf(const char * fmt, ...) { }
    #endif

#endif

#ifdef DFU_ON_DOUBLE_RESET

    #define set_magic_key(a) *(__IO uint32_t *)DFU_MAGIC_KEY_ADDRESS = a; \
                             *(__IO uint32_t *)DFU_MAGIC_KEY_ADDRESS = a

#else

    #define set_magic_key(a)

#endif

// Return value: 0=equal, 1=different, 2=error
uint32_t compareFlashToFile(void)
{
    uint32_t i = 0, j;
    uint32_t file_crc32 = ~CRC32_START; // Invert here, will be undone in crc32b
    int difference_found = 0;
    int different = 0;
    unsigned int bytesRead;
    int result = f_lseek(&fwFile, 0); // Not strictly needed

    while ((i < fileSize) && !result)
    {
        result = f_read(&fwFile, fileBuffer, FILE_BUFFER_SIZE, &bytesRead);
        file_crc32 = crc32b(~file_crc32, fileBuffer, bytesRead);
        j = 0;
        while ((j < bytesRead) && !result)
        {
            if (*(__IO char*)(FLASH_USER_START_ADDR + i + j) != fileBuffer[j])
                difference_found = 1;
            j++;
        }

        if (difference_found)
        {
            uart_printf("*");
            different = 1;
            difference_found = 0; // Reset block different status
        }
        else
            uart_printf("=");

        i += bytesRead;
    }

    if (result)
    {
        uart_printf(" Error\r\nFile read error: %d\r\n", result);
        return 2;
    }
    else
    if (different)
        uart_printf(" Different\r\nFlash contents differs, update is required\r\n");
    else
    {
        uart_printf(" Equal\r\nFlash contents is the same, update is not required\r\n");
        uart_printf("Flash CRC32: 0x%x\r\n", file_crc32);
    }

    return different; // 0=equal, 1=different, 2=file read error
}

// Return value: 0=OK, 1=failed/error
int CopyFileToFlashMemory(void)
{
    // STM32H7xx FLASH SECTORS 0-7 all are 128 KBytes (FLASH_SECTOR_SIZE)

      // Erase required sectors to fit the user application
      uint32_t erasedSize = 0;
      uint32_t sector = FLASH_USER_START_SECTOR;

      HAL_FLASH_Unlock();
      FRESULT result = f_lseek(&fwFile, 0);

      while ((erasedSize < fileSize) && !result)
      {
          uart_printf("Erasing 128KB flash sector %d\r\n", sector);

          FLASH_Erase_Sector(sector, FLASH_BANK_1, FLASH_VOLTAGE_RANGE_3);

          erasedSize += FLASH_SECTOR_SIZE;
          sector++;
      }

      uart_printf("Flashing user application to 0x0%x\r\n", FLASH_USER_START_ADDR);

      uint32_t byteCounter = 0;
      uint32_t i;
      uint32_t file_crc32 = ~CRC32_START; // Invert here, will be undone in crc32b
    unsigned int bytesRead;

      while ((byteCounter < fileSize) && !result)
      {
          // f_read will always return a full buffer, except at the end of the file
          result = f_read(&fwFile, fileBuffer, FILE_BUFFER_SIZE, &bytesRead);
          file_crc32 = crc32b(~file_crc32, fileBuffer, bytesRead);

          if (bytesRead < FILE_BUFFER_SIZE) // Add some "erased flash" bytes to the buffer at the end of the file
              memset(fileBuffer + bytesRead, 0xFF, (FILE_BUFFER_SIZE - bytesRead) % FLASHWORD);

          // Write FLASHWORDs to flash memory
          i = 0;
          while ((i < bytesRead) && !result)
          {
              result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, FLASH_USER_START_ADDR + byteCounter + i, (volatile uint32_t)(fileBuffer + i));
              i += FLASHWORD;
          }
          byteCounter += bytesRead;
          uart_printf("=");
      }

      HAL_FLASH_Lock();


      if (!result) // All went OK, verify flash contents
      {
          uint32_t flash_crc32 = crc32b(CRC32_START, (uint8_t*)FLASH_USER_START_ADDR, fileSize);
        if (file_crc32 != flash_crc32)
        {
              uart_printf("* Verify failed\r\n");
            result = 1; // Signal error
        }
          else
              uart_printf(" Verify OK\r\n");

          uart_printf("Flash CRC32: 0x%x\r\n", flash_crc32);
      }
      else
        uart_printf(" Failed: %d\r\n", result);

      return result;
}

// -----------------------------------------------------------------
// TinyUSB SPI API for MAX3421 USB 2.0 HOST
// -----------------------------------------------------------------
// API to enable/disable MAX3421 INTR pin interrupt
// Continuously called when SPI traffic occurs
void tuh_max3421_int_api(uint8_t rhport, bool enabled)
{
  (void) rhport; // Not used

  if (enabled)
  {
    // Configure MAX3421 interrupt line, pin PE13
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); // Set interrupt priority
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         // Enable EXT interrupt
  }
  else
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);        // Disable EXT interrupt
}

// TinyUSB API to control MAX3421 SPI CS
// Note: Hardware NSS doesn't work with the MAX3421 for some reason
void tuh_max3421_spi_cs_api(uint8_t rhport, bool active)
{
  (void) rhport; // Not used
  HAL_GPIO_WritePin(MAX3421_CS_GPIO_Port, MAX3421_CS_Pin, active ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

// This is a stripped down version of:
// HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);
void HAL_SPI_TransmitReceive_fast(const uint8_t *pTxData, uint8_t *pRxData, uint16_t count)
{
  uint16_t rx_count = count;
  uint16_t tx_count = count;
  static volatile uint8_t dummy;

  volatile uint8_t *TXDR = (uint8_t*) &(SPI_HANDLE.Instance->TXDR);
  volatile uint8_t *RXDR = (uint8_t*) &(SPI_HANDLE.Instance->RXDR);
  volatile uint32_t *SR = &(SPI_HANDLE.Instance->SR);

  // Set the number of data at current transfer
  MODIFY_REG(SPI_HANDLE.Instance->CR2, SPI_CR2_TSIZE, count);

  __HAL_SPI_ENABLE(&SPI_HANDLE);
  SET_BIT(SPI_HANDLE.Instance->CR1, SPI_CR1_CSTART);

  while (tx_count || rx_count)
  {
      if (tx_count && (*SR & SPI_FLAG_TXE))
      {
          if (pTxData)
              *TXDR = *pTxData++;
          else
              *TXDR = 0xFF;
          tx_count--;
      }

      if (rx_count && (*SR & SPI_FLAG_RXNE))
      {
          if (pRxData)
            *pRxData++ = *RXDR;
          else
            dummy = *RXDR;
          rx_count--;
      }
  }

  // Call standard close procedure with error check
  __HAL_SPI_CLEAR_TXTFFLAG(&SPI_HANDLE);
  __HAL_SPI_DISABLE(&SPI_HANDLE);

  UNUSED(dummy);
}

// TinyUSB API to transfer data with MAX3421 SPI
// Either tx_buf or rx_buf can be NULL, which means transfer is write or read only
bool tuh_max3421_spi_xfer_api(uint8_t rhport, uint8_t const * tx_buf, uint8_t* rx_buf, size_t xfer_bytes)
{
  (void) rhport; // Unused

#ifdef FAST_SPI // Faster SPI implementation
  HAL_SPI_TransmitReceive_fast(tx_buf, rx_buf, xfer_bytes);
#else

  if ((tx_buf == NULL) && (rx_buf == NULL))
    return false;

  if (tx_buf == NULL) // Receive only
    return (HAL_SPI_Receive(&SPI_HANDLE, rx_buf, (uint16_t)xfer_bytes, 1000) == HAL_OK);

  if (rx_buf == NULL) // Send only
    return (HAL_SPI_Transmit(&SPI_HANDLE, tx_buf, (uint16_t)xfer_bytes, 1000) == HAL_OK);

  // Send and receive
  HAL_SPI_TransmitReceive(&SPI_HANDLE, tx_buf, rx_buf, xfer_bytes, 1000);

#endif // FAST_SPI

  return true;

}

// TinyUSB API to get time in milliseconds
uint32_t tusb_time_millis_api(void)
{
  return HAL_GetTick();
}

// TinyUSB Callback implementation
static bool inquiry_complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data)
{
  msc_cbw_t const* cbw = cb_data->cbw;
  msc_csw_t const* csw = cb_data->csw;

  if (csw->status != 0)
  {
    uart_printf("Inquiry failed\r\n");
    return false;
  }

  // Get capacity of device
  uint32_t const block_count = tuh_msc_get_block_count(dev_addr, cbw->lun);
  uint32_t const block_size  = tuh_msc_get_block_size (dev_addr, cbw->lun);

  uart_printf("Time: %ums  Disk size: %d MB (%d blocks of %d bytes)\r\n", HAL_GetTick(), block_count / ((1024 * 1024) / block_size), block_count, block_size);

  result = f_mount(&FatFs, "", 1);
  if (result)
  {
      if (result == 3)
             uart_printf("No medium mounted, status: 3\r\n");
      else
             uart_printf("ERROR: USB mounting failed, not FAT/exFAT formatted? Error: %d\r\n", result);

      goto USER_APP;
  }

  uart_printf(FIRMWARE_FILENAME);
  if (f_open(&fwFile, FIRMWARE_FILENAME, FA_READ))
  {
      uart_printf(" not found\r\n");
      goto USER_APP;
  }
  uart_printf(" opened successfully\r\n");

  fileSize = f_size(&fwFile);

  // Get device flash size from memory (in KBytes)
  __IO uint16_t flashSize = *(uint32_t*)(FLASHSIZE_BASE);
  uart_printf("Total flash memory size: %dKB\r\n", flashSize);

  uint32_t freeFlash = (flashSize << 10) - FLASH_BOOTLOADER_SIZE;
  uart_printf("Free flash space: %dKB\r\n", freeFlash >> 10);

  uart_printf("Firmware file size: %dKB\r\n", fileSize >> 10);

  if (fileSize > freeFlash)
  {
      uart_printf("ERROR: Insufficient free flash space, aborting\r\n");
      f_close(&fwFile); // Not strictly needed, comment out to save some flash
      goto USER_APP;
  }

  #ifdef COMPARE_BEFORE_FLASH

        uart_printf("Comparing update to flash contents\r\n");

        result = compareFlashToFile(); // Return value: 0=equal, 1=different, 2=error

        if (result > 1) // File read error
            goto USER_APP;

        if (result == 1) // Flash is different, update required
            result = CopyFileToFlashMemory(); // Return value: 0=OK, 1=failed/error

  #else // Just flash the file

      result = CopyFileToFlashMemory(); // Return value: 0=OK, 1=failed/error

  #endif

  f_close(&fwFile); // Must close file before renaming

  #ifdef FIRMWARE_RENAME

  if (!result) // Only rename/delete if file was flashed successfully
  {
      f_unlink(FIRMWARE_RENAME); // Delete the old firmware (if present)

      if (f_rename(FIRMWARE_FILENAME, FIRMWARE_RENAME) != FR_OK)
      {
          uart_printf("ERROR: Failed to rename firmware file to ");
          result = 1; // Signal error
      }
      else
          uart_printf("Renaming file to ");

      uart_printf(FIRMWARE_RENAME "\r\n");
  }

  #endif

  USER_APP:

    set_magic_key(0); // IMPORTANT, KEEP HERE

    f_mount(NULL, "", 0); // Unmount USB, not strictly needed, comment out to save some flash

    if (*(__IO uint32_t*)FLASH_USER_START_ADDR != 0xFFFFFFFF) // Check if flash is empty
    {
        uart_printf("Starting user application at 0x0%x\r\n", FLASH_USER_START_ADDR);
        HAL_Delay(25);

          tuh_max3421_int_api(0, false);
        HAL_RCC_DeInit(); // Set the clock to the default state
        HAL_DeInit();

        uint32_t *vtor = (void*)FLASH_USER_START_ADDR;
        SCB->VTOR = (uint32_t)vtor;

        // Make the jump
        asm volatile("MSR msp,%0\nbx %1" : : "r"(vtor[0]), "r"(vtor[1]));
    }

    uart_printf("No user application found, done!\r\n");

    while (1)
    { // Start slow LED flash
        #ifdef PROGRESS_LED_PIN
            HAL_GPIO_TogglePin(PROGRESS_LED_PORT, PROGRESS_LED_PIN);
            HAL_Delay(1500);
        #endif
    };

  return true;
}

void tuh_msc_mount_cb(uint8_t dev_addr)
{ // Device mounted
  uint8_t const lun = 0;
  tuh_msc_inquiry(dev_addr, lun, &scsi_resp.inquiry, inquiry_complete_cb, 0);
}


//--------------------------------------------------------------------+
// DiskIO Implementation
//--------------------------------------------------------------------+
static void wait_for_disk_io(BYTE pdrv)
{
  while(_disk_busy)
    tuh_task();
}

static bool disk_io_complete(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data)
{
  (void) dev_addr; (void) cb_data;
  _disk_busy = false;
  return true;
}

DSTATUS disk_status (BYTE pdrv) // Physical drive number should be 0
{
  uint8_t dev_addr = pdrv + 1;
  return tuh_msc_mounted(dev_addr) ? 0 : STA_NODISK;
}

DSTATUS disk_initialize (BYTE pdrv) // Physical drive number should be 0
{
  (void) pdrv;
    return 0; // nothing to do
}

DRESULT disk_read (
    BYTE pdrv,        // Physical drive nmuber to identify the drive
    BYTE *buff,        // Data buffer to store read data
    LBA_t sector,    // Start sector in LBA
    UINT count)        // Number of sectors to read
{
    uint8_t const dev_addr = pdrv + 1;
    uint8_t const lun = 0;

    _disk_busy = true;
    tuh_msc_read10(dev_addr, lun, buff, sector, (uint16_t) count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);

    return RES_OK;
}

#if (FF_FS_READONLY == 0)
DRESULT disk_write (
    BYTE pdrv,        /* Physical drive number to identify the drive */
    const BYTE *buff, /* Data to be written */
    LBA_t sector,     /* Start sector in LBA */
    UINT count)       /* Number of sectors to write */
{
    uint8_t const dev_addr = pdrv + 1;
    uint8_t const lun = 0;

    _disk_busy = true;
    tuh_msc_write10(dev_addr, lun, buff, sector, (uint16_t) count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);

    return RES_OK;
}
#endif

DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive number (0..) */
    BYTE cmd,        /* Control code */
    void *buff)        /* Buffer to send/receive control data */
{
  uint8_t const dev_addr = pdrv + 1;
  uint8_t const lun = 0;
  switch ( cmd )
  {
    case CTRL_SYNC:
      // nothing to do since we do blocking
      return RES_OK;

    case GET_SECTOR_COUNT:
      *((DWORD*) buff) = (WORD) tuh_msc_get_block_count(dev_addr, lun);
      return RES_OK;

    case GET_SECTOR_SIZE:
      *((WORD*) buff) = (WORD) tuh_msc_get_block_size(dev_addr, lun);
      return RES_OK;

    case GET_BLOCK_SIZE:
      *((DWORD*) buff) = 1;    // erase block size in units of sector size
      return RES_OK;

    default:
      return RES_PARERR;
  }

  return RES_OK;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  #define rHIRQ         (25 << 3)
  #define bmFRAMEIRQ    0x40
  bool MAX3421_wait_frames(uint32_t frames, uint32_t timeout)
  {
    tuh_max3421_reg_write(0, rHIRQ, bmFRAMEIRQ, false); // clear the FRAME interrupt
      uint32_t start = HAL_GetTick();
      while (frames && (HAL_GetTick() - start < timeout))
      {
           if (tuh_max3421_reg_read(0, rHIRQ, false) & bmFRAMEIRQ)
           {
          tuh_max3421_reg_write(0, rHIRQ, bmFRAMEIRQ, false); // clear the FRAME interrupt
          frames--;
           }
      }
//    tuh_task();
    return (frames > 0);
  }

  tuh_max3421_reg_read(0, (18 << 3), false); // Startup SPI dummy command, read chip revision

  tusb_rhport_init_t host_init = { .role = TUSB_ROLE_HOST, .speed = TUSB_SPEED_AUTO };
  tusb_init(BOARD_TUH_RHPORT, &host_init);

#ifdef DFU_ON_DOUBLE_RESET

    // Detect magic key
    if (*(__IO uint32_t*)DFU_MAGIC_KEY_ADDRESS == DFU_MAGIC_KEY)
    {
        set_magic_key(0);
        MX_GPIO_Init();
        MX_USART1_UART_Init();
        uart_printf("\r\nStarting DFU mode\r\n");
        HAL_Delay(25);

        tuh_max3421_int_api(0, false);
        HAL_RCC_DeInit(); // Set the clock to the default state
        HAL_DeInit();

        uint32_t *vtor = (void*)DFU_BOOTLOADER_ADDRESS;
        SCB->VTOR = (uint32_t)vtor;

        // Make the jump
        asm volatile("MSR msp,%0\nbx %1" : : "r"(vtor[0]), "r"(vtor[1]));
    }

      // Wait for 2nd reset while DFU marker is set
    set_magic_key(DFU_MAGIC_KEY);

#endif

  bool dontwait = MAX3421_wait_frames(250, 2500); // Number of frames (about 2ms/frame), timeout in ms

#ifdef DFU_ON_DOUBLE_RESET

    set_magic_key(0);

#endif

    uart_printf("MAX3421 USB bootloader started\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    tuh_task();

    // Timeout counter
    if (dontwait | (HAL_GetTick() > 2200)) // Usually takes 1150-2200ms
    {
        uart_printf("Time: %ums   Timeout...\r\n", HAL_GetTick());
        set_magic_key(0); // IMPORTANT, KEEP HERE

        f_mount(NULL, "", 0); // Unmount USB, not strictly needed, comment out to save some flash

        if (*(__IO uint32_t*)FLASH_USER_START_ADDR != 0xFFFFFFFF) // Check if flash is empty
        {
            uart_printf("Starting user application at 0x0%x\r\n", FLASH_USER_START_ADDR);
            HAL_Delay(25);

            tuh_max3421_int_api(0, false);
            HAL_RCC_DeInit(); // Set the clock to the default state
            HAL_DeInit();

            uint32_t *vtor = (void*)FLASH_USER_START_ADDR;
            SCB->VTOR = (uint32_t)vtor;

            // Make the jump
            asm volatile("MSR msp,%0\nbx %1" : : "r"(vtor[0]), "r"(vtor[1]));
        }

        uart_printf("No user application found, done!\r\n");

        while (1)
        { // Start slow LED flash
            #ifdef PROGRESS_LED_PIN
                HAL_GPIO_TogglePin(PROGRESS_LED_PORT, PROGRESS_LED_PIN);
                HAL_Delay(1500);
            #endif
        };
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 44;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 11;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_15CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_15CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 250000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MAX3421_CS_GPIO_Port, MAX3421_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX3421_INTR_Pin */
  GPIO_InitStruct.Pin = MAX3421_INTR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MAX3421_INTR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX3421_CS_Pin */
  GPIO_InitStruct.Pin = MAX3421_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MAX3421_CS_GPIO_Port, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(MAX3421_INTR_EXTI_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(MAX3421_INTR_EXTI_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
