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
* flash_amd.c                                                                 *
*                                                                             *
* Programming algorithms and associated functions for AMD flash parts         *
*                                                                             *
* Author PRR                                                                  *
*                                                                             *
******************************************************************************/

#include <errno.h>
#include <io.h>
#include <unistd.h>
#include "alt_types.h"
#include "altera_avalon_cfi_flash.h"
#include "altera_avalon_cfi_flash_funcs.h"

/*
 * Prototypes local to this file
 */
static int alt_wait_for_command_to_complete_amd( alt_flash_cfi_dev* flash, 
                                      int offset, 
                                      alt_u8 data);
static int alt_write_word_amd(  alt_flash_cfi_dev* flash, 
                                const int offset, 
                                const alt_u8* src_addr);

/******************************************************************************
*                                                                             *
* Functions used as the interface to a flash programming algorithm by the     *                                                                           *
* common flash code                                                           *
*                                                                             *
******************************************************************************/

/*
 * alt_program_amd
 * 
 * Program a block (or part of one)
 * 
 * Use the old fashioned word at a time program method
 * that way it will work on all devices
 *
 *  There is another mechanism which puts the device into a mode and
 * then writes Addr, Data Addr, Data etc.
 */
int alt_program_amd(alt_flash_dev* flash_info, int block_offset, 
                int offset, const void* src_addr, 
                int length)
{
  int ret_code = 0;
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;
  
  
  ret_code = alt_flash_program_block( flash, offset, src_addr, length, 
                                    alt_write_word_amd);
  return ret_code;
}


/*
 * alt_erase_block_amd
 * 
 * Erase the selected erase block
 */
int alt_erase_block_amd(alt_flash_dev* flash_info, int block_offset)
{
  int   ret_code = 0;
  int   timeout;
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;
  volatile alt_u8  value;

  (*flash->write_command)(flash->dev.base_addr, 0x555, (alt_u8)0xAA);
  (*flash->write_command)(flash->dev.base_addr, 0x2AA, (alt_u8)0x55);
  (*flash->write_command)(flash->dev.base_addr, 0x555, (alt_u8)0x80);
  (*flash->write_command)(flash->dev.base_addr, 0x555, (alt_u8)0xAA);
  (*flash->write_command)(flash->dev.base_addr, 0x2AA, (alt_u8)0x55);

  (*flash->write_native)((alt_u8*)flash->dev.base_addr+block_offset, 0x30);

  /*
   * Delay to meet AM29LV065D timing requirements
   */
  usleep(10000);
  
  /*
   * Bit 3 indicates that the erase command has been accepted
   * this last 50µS
   */
  timeout = 50;   
  do 
  {
    value = IORD_8DIRECT((alt_u8*)flash->dev.base_addr + block_offset, 0);
    usleep(1000);
    timeout--;
  }while(!(value & 0x8) && (timeout > 0));


  timeout = flash->erase_timeout;
  
  /*
   *  Bit 7 goes low until the block is erased if bit 5 goes to 
   *  1 it's an error
   */
  while (timeout > 0)
  {
    value = IORD_8DIRECT((alt_u8*)flash->dev.base_addr + block_offset, 0);
    if ((value & 0x80) || (value &0x20))
    {
      break;
    }
    usleep(1000);
    timeout -= 1000;
  }
  
  if (timeout <= 0)
  {
    ret_code = -ETIMEDOUT;
  }
  else
  {
    value = IORD_8DIRECT((alt_u8*)flash->dev.base_addr + block_offset, 0);
    if (!(value & 0x80))
    {
      ret_code = -EIO;
    }
  }    
  
  return ret_code;
}

/******************************************************************************
*                                                                             *
* Private functions for the AMD algorihtm                                     *
* common flash code                                                           *
*                                                                             *
******************************************************************************/

/*
 * alt_wait_for_command_to_complete_amd
 * 
 * Wait for a program command to finish on an amd device
 */
 
int alt_wait_for_command_to_complete_amd(alt_flash_cfi_dev* flash,
                                         int offset, 
                                          alt_u8 data)
{
  volatile alt_u8  value;
  int timeout = flash->write_timeout * 100;
  int ret_code = 0;
  
  value = IORD_8DIRECT(flash->dev.base_addr, offset);
  while (timeout > 0)
  {
    if (((value & 0x80 ) == (data &0x80)) ||
        (value & 0x20))
    {
      break;
    }
    usleep (1);
    timeout--;
    value = IORD_8DIRECT(flash->dev.base_addr, offset);
  }
  
  if (timeout == 0)
  {
    ret_code = -ETIMEDOUT;
  }
  else
  {
    value = IORD_8DIRECT(flash->dev.base_addr, offset);
    if ((value & 0x80) != (data&0x80))
    {
      ret_code = -EIO;
    }
  }    
  return ret_code;
}

static int alt_write_word_amd(  alt_flash_cfi_dev* flash, 
                                const int offset, 
                                const alt_u8* src_addr)
{
  int ret_code = 0;
  alt_u8 value;
  (*flash->write_command)(flash->dev.base_addr, 0x555, (alt_u8)0xAA);
  (*flash->write_command)(flash->dev.base_addr, 0x2AA, (alt_u8)0x55);
  (*flash->write_command)(flash->dev.base_addr, 0x555, (alt_u8)0xA0);
  
  value = *src_addr;

  alt_write_value_to_flash(flash, offset, src_addr);
  
  ret_code = alt_wait_for_command_to_complete_amd(flash, 
                                                  offset,
                                                  value);
  return ret_code;
  
}
