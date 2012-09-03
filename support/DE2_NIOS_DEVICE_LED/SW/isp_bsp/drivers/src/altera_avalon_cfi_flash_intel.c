/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2003-2004 Altera Corporation, San Jose, California, USA.      *
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
* altera_avalon_cfi_flash_intel.c                                             *
*                                                                             *
* Programming algorithms and associated functions for Intel and Intel Strata  *
* flash parts                                                                 *
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

static int alt_write_word_intel(  alt_flash_cfi_dev* flash, 
                                  const int offset, 
                                  const alt_u8* src_addr);
static int alt_unlock_block_intel(alt_flash_cfi_dev* flash, int block_offset);


/******************************************************************************
*                                                                             *
* Functions used as the interface to a flash programming algorithm by the     *                                                                           *
* common flash code                                                           *
*                                                                             *
******************************************************************************/

/*
 * alt_program_intel
 * 
 * Program a block (or part of one)
 */
int alt_program_intel(alt_flash_dev* flash_info, int block_offset, 
                int offset, const void* src_addr, 
                int length)
{
  int ret_code = 0;
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;
  
   /*
  * If this block is locked then unlock it
  */
  ret_code = alt_unlock_block_intel(flash, block_offset);

  if (!ret_code)
  {

    ret_code = alt_flash_program_block( flash, offset, src_addr, length, 
                                        alt_write_word_intel);
  }
  
  return ret_code;
}

/*
 * alt_erase_block_intel
 * 
 * Erase the selected erase block
 */
int alt_erase_block_intel(alt_flash_dev* flash_info, int block_offset)
{
  int   ret_code = 0;
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;
  volatile alt_u8  status;
  int   timeout = flash->erase_timeout;

  /*
  * If this block is locked then unlock it
  */
  ret_code = alt_unlock_block_intel(flash, block_offset);

  if (!ret_code)
  {

    flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0x20);
    flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0xD0);

    do
    {
      status = IORD_8DIRECT(flash->dev.base_addr, block_offset);
      if (status & 0x80)
      {
        break;
      }
      usleep(1000);
      timeout -= 1000;
    }while(timeout > 0);
    
    if (timeout <= 0)
    {
      ret_code = -ETIMEDOUT;
    }
    else if (status & 0x7f)
    {
      /* If we have an error of some kind bomb out */
      ret_code = -EIO;
      status = IORD_8DIRECT(flash->dev.base_addr, block_offset);
    }

    /* Put the device back into read array mode */
    flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0xFF);
  }
  
  return ret_code;
}

/*
* Private Intel specific functions
*/

static int alt_unlock_block_intel(alt_flash_cfi_dev* flash, int block_offset)
{
  alt_u8  locked;
  alt_u8  status;
  int ret_code = 0;
  int timeout = flash->write_timeout * 100;


  /*
  * Is this block locked?
  */
  flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0x90);
  locked = IORD_8DIRECT(flash->dev.base_addr, block_offset + 4);
  if (locked & 0x1)
  {
    flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0x60);
    flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0xD0);

    do
    {
      status = IORD_8DIRECT(flash->dev.base_addr, block_offset);
      if (status & 0x80)
      {
        break;
      }
      timeout--;
      usleep(1);
    }while(timeout > 0);

    if (timeout == 0)
    {
      ret_code = -ETIMEDOUT;
    }
    else if (status & 0x7f)
    {
      /* If we have an error of some kind bomb out */
      ret_code = -EIO;
    }
  }

  /*
  * Back to Read Array mode
  */
  flash->write_native((alt_u8*)flash->dev.base_addr + block_offset, 0xFF);

  return ret_code;
}

/*
 * alt_write_word_intel
 * 
 * Program the native word size of the flash from src_addr
 * offset bytes into the flash
 */
 
int alt_write_word_intel( alt_flash_cfi_dev* flash, 
                                  const int offset, const alt_u8* src_addr)
{ 
  int ret_code = 0;
  alt_u8 status;
  (*flash->write_native)((alt_u8*)flash->dev.base_addr+offset, 0x40);
  alt_write_value_to_flash(flash, offset, src_addr);

  do
  {
    status = IORD_8DIRECT(flash->dev.base_addr, offset);
  }while(!(status & 0x80));

  /* If we have an error of some kind bomb out */
  if (status & 0x7f)
  {
    ret_code = -EIO;
  }

  /* Put the device back into read array mode */
  flash->write_native((alt_u8*)flash->dev.base_addr + offset, 0xFF);
  
  return ret_code;
}
