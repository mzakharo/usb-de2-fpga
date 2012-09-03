#ifndef __ALT_FLASH_CFI_H__
/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2003 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
* altera_avalon_cfi_flash.h                                                   *
*                                                                             *
* Public interfaces to the Altera flash programming library for common flash  *
* interface flash parts                                                       *
*                                                                             *
*                                                                             *
* Author PRR                                                                  *
*                                                                             *
******************************************************************************/
#define __ALT_FLASH_CFI_H__
#include "alt_types.h"
#include "sys/alt_flash_dev.h"
#include "sys/alt_llist.h"

/******************************************************************************
*                                                                             *
* Structures                                                                  *
*                                                                             *
******************************************************************************/

typedef struct alt_flash_cfi_dev alt_flash_cfi_dev;

/* 
 * Function pointers for performing operations on the flash
 */
typedef void (*alt_write_command_fn)(void* base_address, int offset, alt_u8 value);
typedef alt_u8 (*alt_read_query_fn)(alt_flash_cfi_dev* flash, int address);
typedef void (*alt_write_native_fn)(void* address, alt_u32 value);
typedef int (*alt_program_word_fn)(alt_flash_cfi_dev* flash, const int offset, 
                              const alt_u8* src_addr);

/*
 *  Description of the flash
 */
struct  alt_flash_cfi_dev
{
  alt_flash_dev dev;
  int           algorithm;
  int           mode_width;
  int           device_width;
  int           write_timeout;
  int           erase_timeout;
  int           primary_address;
    
  /* 
   * Flash type specific functions to access the flash and perform various
   * operations
   */
  alt_write_command_fn  write_command;
  alt_read_query_fn     read_query;
  alt_write_native_fn   write_native;
};

/*
 *  The initialisation function which reads the CFI table and fills out
 *  the appropriate sections of the the alt_flash__cfi_dev structure
 */

int alt_flash_cfi_init( alt_flash_cfi_dev* flash );

/*
 *  Functions exported through the common Flash interface
 */                

/*
 * Restrictions - The alt_flash_cfi_write function programs will erase 
 * any sectors it needs to. However it will not preserve the existing
 * contents of areas of that sector which are not being written to.
 * If you wish to have all the contents of the sector preserved you 
 * the user need to be aware of this and read out 
 * the contents of that sector and add it to the data you wish to program. 
 * The reasoning here is that sectors can be very large eg. 64k which is a 
 * large buffer to tie up in our programming library, when not all users will 
 * want that functionality.
 */

int alt_flash_cfi_write( alt_flash_dev* flash_info, int offset, 
                          const void* src_addr, int length );

int alt_flash_cfi_read( alt_flash_dev* flash_info, int offset, 
                        void* dest_addr, int length );
int alt_flash_cfi_get_info( alt_flash_fd* fd, flash_region** info, 
                            int* number_of_regions);

/*
 * Macro used to determine if a particluar flash device is being used to
 * store code. If it is, then the driver is not initialised for that device.
 * This avoids the risk of trying to write to a part while executing code from 
 * it. 
 */  

#define __ALT_CFI_DEV_IN_USE(base, text, rodata, exc) \
  ((base == text##_BASE) || (base == rodata##_BASE) || (base == exc##_BASE))

#define _ALT_CFI_DEV_IN_USE(base, text, rodata, exc) __ALT_CFI_DEV_IN_USE(base, text, rodata, exc)

#define ALT_CFI_DEV_IN_USE(base) _ALT_CFI_DEV_IN_USE(base,              \
                                                     ALT_TEXT_DEVICE,   \
                                                     ALT_RODATA_DEVICE, \
                                                     ALT_EXCEPTIONS_DEVICE)

/* 
*   Macros used by alt_sys_init.c
*
* By default this driver is initialised in alt_sys_init. However if the 
* "small driver" feature has been selected, or if fast simulation has been requested
* no initialisation is performed.
*
* This causes the driver to be excluded from the system (unless explicitly 
* initialised by the user from main()), and therefore reduces code footprint.
*
*/

#if (!defined(ALT_EXCLUDE_CFI_FLASH) && ((!defined(ALT_USE_SMALL_DRIVERS) && !defined(ALT_SIM_OPTIMIZE)) || defined(ALT_USE_CFI_FLASH)))
#define ALTERA_AVALON_CFI_FLASH_INSTANCE(name, dev)                        \
static alt_flash_cfi_dev dev =                                             \
{                                                                          \
  {                                                                        \
    ALT_LLIST_ENTRY,                                                       \
    name##_NAME,                                                           \
    NULL,                                                                  \
    NULL,                                                                  \
    ALT_CFI_DEV_IN_USE(name##_BASE) ? NULL : alt_flash_cfi_write,          \
    ALT_CFI_DEV_IN_USE(name##_BASE) ? NULL : alt_flash_cfi_read,           \
    ALT_CFI_DEV_IN_USE(name##_BASE) ? NULL : alt_flash_cfi_get_info,       \
    NULL,                                                                  \
    NULL,                                                                  \
    ((void*)( name##_BASE)),                                               \
    name##_SIZE                                                            \
  }                                                                        \
}

/*
 * No driver is provided if the device is being used to store any of the code
 * sections. This prevents the possibility of trying to run from flash while
 * programming it.
 */

#define ALTERA_AVALON_CFI_FLASH_INIT(name, dev)       \
  if (!ALT_CFI_DEV_IN_USE(name##_BASE))               \
  {                                                   \
    alt_flash_cfi_init (&dev );                       \
  }

#else /* exclude driver */

#define ALTERA_AVALON_CFI_FLASH_INSTANCE(name, dev) extern int alt_no_storage
#define ALTERA_AVALON_CFI_FLASH_INIT(name, dev) while (0)

#endif

#endif /* __ALT_FLASH_CFI_H__ */
