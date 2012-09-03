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
* alt_flash_cfi.c                                                             *
*                                                                             *
* API for programming CFI compliant devices  - This interface is basically    *
* A POSIX device driver interface                                             *
*                                                                             *
* Author PRR                                                                  *
*                                                                             *
******************************************************************************/
#include <errno.h>
#include <io.h>
#include <string.h>
#include "sys/param.h"
#include "alt_types.h"
#include "sys/alt_cache.h"
#include "altera_avalon_cfi_flash.h"
#include "altera_avalon_cfi_flash_amd_funcs.h"
#include "altera_avalon_cfi_flash_intel_funcs.h"
#include "altera_avalon_cfi_flash_funcs.h"

/*
 * alt_flash_cfi_init
 * 
 * Read the CFI table and fill out the alt_flash_cfi_dev structure with all the 
 * information we need to program the flash.
 * 
 */
int alt_flash_cfi_init( alt_flash_cfi_dev* flash  )
{
  int ret_code = 0;
 
  ret_code = alt_read_cfi_width( flash );
  
  if (!ret_code)
    ret_code = alt_set_flash_width_func( flash );
  
  if (!ret_code)
    ret_code = alt_read_cfi_table( flash );

  if (!ret_code) 
    ret_code = alt_set_flash_algorithm_func( flash);

  /*
  *  Register this device as a valid flash device type
  */ 
  if (!ret_code)
    ret_code = alt_flash_device_register(&(flash->dev));
 
  return ret_code;
}


/*
 * alt_flash_cfi_write
 * 
 * Program the data into the flash at the selected address. 
 * 
 * Restrictions - For now this function will program the sectors it needs, 
 * if it needs to erase a sector it will. If you wish to have all the contents
 * of the sector preserved you the user need to be aware of this and read out 
 * the contents of that sector and add it to the data you wish to program. 
 * The reasoning here is that sectors can be very large eg. 64k which is a 
 * large buffer to tie up in our programming library, when not all users will 
 * want that functionality.
 */
 int alt_flash_cfi_write( alt_flash_dev* flash_info, int offset, 
                          const void* src_addr, int length )
 {
  int         ret_code = 0;
  int         i,j;
  int         data_to_write;
  int         full_length = length;
  int         current_offset;
  int         start_offset = offset;
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;

  /*
   * First and foremost which sectors are affected?
   */   
  for(i=0;i<flash->dev.number_of_regions;i++)
  {
    /* Is it in this erase block region?*/
    if((offset >= flash->dev.region_info[i].offset) &&
      (offset < (flash->dev.region_info[i].offset + 
      flash->dev.region_info[i].region_size)))
    {
      current_offset = flash->dev.region_info[i].offset;

      for(j=0;j<flash->dev.region_info[i].number_of_blocks;j++)
      {
        if ((offset >= current_offset ) && 
            (offset < (current_offset + 
            flash->dev.region_info[i].block_size)))
        {
          /*
           * Check if the contents of the block are different
           * from the data we wish to put there
           */
          data_to_write = ( current_offset + flash->dev.region_info[i].block_size 
                            - offset); 
          data_to_write = MIN(data_to_write, length);
          if(memcmp(src_addr, 
                    (alt_u8*)flash->dev.base_addr+offset,
                    data_to_write))
          {
            ret_code = (*flash->dev.erase_block)( &flash->dev, current_offset);

            if (!ret_code)
            {
              ret_code = (*flash->dev.write_block)( 
                                                  &flash->dev, 
                                                  current_offset, 
                                                  offset,
                                                  src_addr,
                                                  data_to_write);
            }
          }    
    
          /* Was this the last block? */    
          if ((length == data_to_write) || ret_code)
          {
            goto finished;
          }
          
          length -= data_to_write;
          offset = current_offset + flash->dev.region_info[i].block_size;
          src_addr = (alt_u8*)src_addr + data_to_write;
        }
        current_offset += flash->dev.region_info[i].block_size;
      }     
    } 
  }
finished:    

  alt_dcache_flush((alt_u8*)flash->dev.base_addr+start_offset, full_length);
  return ret_code;
}

/*
 *  alt_flash_cfi_get_info
 * 
 *  Pass the table of erase blocks to the user
 */
int alt_flash_cfi_get_info( alt_flash_fd* fd, flash_region** info, 
                            int* number_of_regions)
{
  int ret_code = 0;
  alt_flash_dev* flash = (alt_flash_dev*)fd;

  *number_of_regions = flash->number_of_regions;

  if (!flash->number_of_regions)
  {
    ret_code = -EIO;
  }
  else if (flash->number_of_regions > ALT_MAX_NUMBER_OF_FLASH_REGIONS)
  {
    ret_code = -ENOMEM;
  }
  else
  {
    *info = &flash->region_info[0];
  }

  return ret_code;
}

/*
 *  Read from an area in flash, you could use memcopy yourself
 *  for these flash types, but we're trying to be generic and future proof
 */
int alt_flash_cfi_read( alt_flash_dev* flash_info, int offset, 
                        void* dest_addr, int length )
{
  alt_flash_cfi_dev* flash = (alt_flash_cfi_dev*)flash_info;
  memcpy(dest_addr, (alt_u8*)flash->dev.base_addr+offset, length);
  return 0;
}

/*
* alt_write_value_to_flash
*
* This function is used by both the intel and amd algorithms
* It writes to the flash described offset from the base of that flash
* It writes the largest word size that the flash can support
* so if it's an 8 bit flash it writes bytes
* 16 bit half word etc.
*/
void alt_write_value_to_flash(alt_flash_cfi_dev* flash, int offset, const alt_u8* src_addr)
{
  alt_u16 half_word_value;
  alt_u32 word_value;

  if (flash->mode_width == 1)
  {
    IOWR_8DIRECT(flash->dev.base_addr, offset, *src_addr);
  }
  else if (flash->mode_width == 2)
  {
    half_word_value = (alt_u16)(*src_addr);
    half_word_value |= (alt_u16)(*(src_addr + 1)) << 8;
    IOWR_16DIRECT(flash->dev.base_addr, offset, half_word_value);
  }
  else if (flash->mode_width == 4)
  {
    word_value = (alt_u32)(*src_addr);
    word_value |= ((alt_u32)(*(src_addr + 1)) << 8);
    word_value |= ((alt_u32)(*(src_addr + 2)) << 16);
    word_value |= ((alt_u32)(*(src_addr + 3)) << 24);
    IOWR_32DIRECT(flash->dev.base_addr, offset, word_value);
  }

  return;
}

/*
* alt_flash_program_block
*
* This function has all the commonality for writing data to the flash
* for example dealing with writes which do not start or finish on 
* a boundary of the natural flash width.
*
* The parameters are;
* flash 
* offset - the offset from the base of flash that we wish to start programming to
* src_addr - pointer to the data we wish to write
* length - the amount of data in bytes we wish to write
* program_word_func - This is an alogorithm specific function which writes the 
* width of the mode that the flash is being used in 
* 8 bits wide it writes a byte, 
* 16 bits  wide it write a half word etc
*
*/
int alt_flash_program_block(  alt_flash_cfi_dev* flash, const int offset, 
                        const alt_u8* src_addr, 
                        const int length,
                        alt_program_word_fn program_word_func)
{
  int     ret_code = 0;
  int     bytes_to_preserve;
  int     unaligned_bytes;
  int     unaligned_end_bytes;
  int     i,j;
  alt_u8  unaligned[4];

  /*
   * First take care of any writes which are on none native boundaries
   * as far as the flash is concerned.
   */
  bytes_to_preserve = ((int)((alt_u8*)flash->dev.base_addr + offset) % 
                      flash->mode_width);
  if ( bytes_to_preserve != 0)
  {
    unaligned_bytes = flash->mode_width - bytes_to_preserve;
    /*
     * Read the bytes we wish to preserve out of flash
     */
    for (i=0;i<bytes_to_preserve;i++)
    {
      unaligned[i] = IORD_8DIRECT(flash->dev.base_addr, 
                                offset-bytes_to_preserve+i);
    }

    for (i=0;i<unaligned_bytes;i++)
    {
      unaligned[bytes_to_preserve + i] = *(alt_u8*)(src_addr + i);
    }
    
    ret_code = (*program_word_func)(flash, offset-bytes_to_preserve, unaligned);
    i = unaligned_bytes;
  }
  else
  {
    i = 0;
  }

  unaligned_end_bytes = (offset+length) % flash->mode_width;
  while ((ret_code == 0) && (i < (length-unaligned_end_bytes)))
  {
    ret_code = (*program_word_func)(flash, offset+i, src_addr+i);
    i += flash->mode_width;     
  }

  /*
   * Now take care of any writes at the end of the buffer which are on none 
   * native boundaries as far as the flash is concerned.
   */
  if (unaligned_end_bytes && !ret_code)
  {
    bytes_to_preserve = flash->mode_width - unaligned_end_bytes;
    
    for (j=0;j<unaligned_end_bytes;j++)
    {
      unaligned[j] = *(alt_u8*)(src_addr+i+j);
    }
    
    for (j=0;j<bytes_to_preserve;j++)
    {
      unaligned[unaligned_end_bytes+j] = IORD_8DIRECT(flash->dev.base_addr, 
                                                      offset+length+j);
    }

    ret_code = (*program_word_func)(flash, offset+i, unaligned);      
  }

  return ret_code;
}

