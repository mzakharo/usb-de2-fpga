/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2007      Altera Corporation, San Jose, California, USA.      *
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
* alt_flash_cfi_table.c                                                       *
*                                                                             *
* Functions for accessing the CFI Table                                       *
*                                                                             *
* Author PRR                                                                  *
*                                                                             *
******************************************************************************/
#include <errno.h>
#include <io.h>
#include "alt_types.h"
#include "altera_avalon_cfi_flash.h"
#include "altera_avalon_cfi_flash_amd_funcs.h"
#include "altera_avalon_cfi_flash_intel_funcs.h"

#define READ_ARRAY_MODE   (alt_u8)0xFF
#define QUERY_MODE        (alt_u8)0x98

#define QUERY_ADDR              0x10 
#define PRIMARY_ADDR            0x15
#define INTERFACE_ADDR          0x28

#define BOTTOM_BOOT_DEVICE        2
#define TOP_BOOT_DEVICE           3

#define CFI_ALG_INTEL_STRATA      1
#define CFI_ALG_AMD               2
#define CFI_ALG_INTEL             3

/*
* Prototypes
*/
int alt_check_primary_table(alt_flash_cfi_dev* flash);

/*
 *  Read an 8 bit value from the CFI query table in flash
 */
alt_u8 alt_read_query_entry_8bit( alt_flash_cfi_dev* flash, int address)
{
  return IORD_8DIRECT((alt_u8*)flash->dev.base_addr, address);
}

alt_u8 alt_read_query_entry_16bit( alt_flash_cfi_dev* flash, int address)
{
  return (IORD_16DIRECT((alt_u8*)flash->dev.base_addr, address*2) & 0xff);
}

alt_u8 alt_read_query_entry_32bit( alt_flash_cfi_dev* flash, int address)
{
  return (IORD_32DIRECT((alt_u8*)flash->dev.base_addr, address*4) & 0xff);
}

/*
 * Write an 8 bit command to a flash
 */
void alt_write_flash_command_8bit_device_8bit_mode( void* base_addr, int offset, alt_u8 value)
{
  IOWR_8DIRECT(base_addr, offset, value);
  return;
}

void alt_write_flash_command_16bit_device_8bit_mode( void* base_addr, int offset, alt_u8 value)
{
  if (offset % 2)
  {
    IOWR_8DIRECT(base_addr, offset*2, value);
  }
  else
  {
    IOWR_8DIRECT(base_addr, (offset*2)+1, value);
  }
  return;
}

void alt_write_flash_command_32bit_device_8bit_mode( void* base_addr, int offset, alt_u8 value)
{
  IOWR_8DIRECT(base_addr, offset*4, value);
  return;
}

void alt_write_flash_command_16bit_device_16bit_mode( void* base_addr, int offset, alt_u8 value)
{
  IOWR_16DIRECT(base_addr, offset*2, ((alt_u16)value)& 0x00ff);
  return;
}

void alt_write_flash_command_32bit_device_16bit_mode( void* base_addr, int offset, alt_u8 value)
{
  IOWR_16DIRECT(base_addr, offset*4, ((alt_u16)value)& 0x00ff);
  return;
}

void alt_write_flash_command_32bit_device_32bit_mode( void* base_addr, int offset, alt_u8 value)
{
  IOWR_32DIRECT(base_addr, offset*4, ((alt_u32)value)& 0x000000ff);
  return;
}

/*
 * Write the value passed to the flash
 */
void alt_write_native_8bit( void* address, alt_u32 value)
{
  IOWR_8DIRECT(address, 0, (alt_u8)(value&0xff));
  return;
}

void alt_write_native_16bit( void* address, alt_u32 value)
{
  IOWR_16DIRECT(address, 0, ((alt_u16)value)& 0xffff);
  return;
}

void alt_write_native_32bit( void* address, alt_u32 value)
{
  IOWR_32DIRECT(address, 0, value);
  return;
}

/*
 * alt_set_flash_width_func
 * 
 * Setup the function pointers for writing a byte to the flash for the width
 * of the device
 */
int alt_set_flash_width_func( alt_flash_cfi_dev* flash)
{ 
  int ret_code = 0;
  
  switch(flash->mode_width)
  {
    case 1:
    {
      flash->write_native = alt_write_native_8bit;

      if (flash->device_width == 1)
      {
        flash->write_command = alt_write_flash_command_8bit_device_8bit_mode;
      }
      else if (flash->device_width == 2)
      {
        flash->write_command = alt_write_flash_command_16bit_device_8bit_mode;
      }
      else if (flash->device_width == 4)
      {
        flash->write_command = alt_write_flash_command_32bit_device_8bit_mode;
      }
      break;
    }
    case 2:
    {
      flash->write_native = alt_write_native_16bit;

      if (flash->device_width == 2)
      {
        flash->write_command = alt_write_flash_command_16bit_device_16bit_mode;
      }
      else if (flash->device_width == 4)
      {
        flash->write_command = alt_write_flash_command_32bit_device_16bit_mode;
      }

      break;
    }
    case 4:
    {
      flash->write_native = alt_write_native_32bit;
      flash->write_command = alt_write_flash_command_32bit_device_32bit_mode;
      break;
    }
    default:
    {
      ret_code = -EACCES;
    }
  }

  if (!ret_code)
  {
    switch(flash->device_width)
    {
      case 1:
      {
        flash->read_query = alt_read_query_entry_8bit;
        break;
      }
      case 2:
      {
        flash->read_query = alt_read_query_entry_16bit;
        break;
      }
      case 4:
      {
        flash->read_query = alt_read_query_entry_32bit;
        break;
      }
      default:
      {
        ret_code = -EACCES;
      }
    }
  }

  return ret_code;
}

/*
 * alt_set_flash_algorithm_func
 * 
 * Setup the function pointers to the functions for this algorithm
 * 
 */
int alt_set_flash_algorithm_func( alt_flash_cfi_dev* flash)
{
  int ret_code = 0;
 
  switch(flash->algorithm)
  {
    case CFI_ALG_AMD:
    {
      flash->dev.erase_block = alt_erase_block_amd;
      flash->dev.write_block = alt_program_amd;
      break;
    }
    case CFI_ALG_INTEL:
    case CFI_ALG_INTEL_STRATA:
    {
      flash->dev.erase_block = alt_erase_block_intel;
      flash->dev.write_block = alt_program_intel;
      break;
    }
    default:
    {
      ret_code = -EIO;
    }
  } 
  return ret_code;  
}


/*
 * read_16bit_query_entry
 * 
 * Read a 16 bit entry from the CFI Query table
 */
static alt_u16 alt_read_16bit_query_entry(alt_flash_cfi_dev* flash, int address)
{
  alt_u16 ret_code;

  ret_code = (*flash->read_query)( flash, address);
  ret_code |= (((int)(*flash->read_query)(flash, address+1)) << 8);                   

  return ret_code;
}


/*
 * read_cfi_table
 * 
 * Read the CFI Table
 */
int alt_read_cfi_table(alt_flash_cfi_dev* flash)
{
  int   i,j;
  int   device_size;
  int   ret_code = 0;
  int   size = 0;
  int   swap;
  int   typical_timeout;
  int   max_timeout;
  int   offset = 0;
  alt_u8  boot_mode;
   
  /*
  * Check that the Primary Vendor Specific table
  * starts with the letters PRI                                                         
  */
  ret_code = alt_check_primary_table(flash);

  if (!ret_code)
  {
    flash->algorithm = (*flash->read_query)(flash, 0x13);
  
    /* 
     * Let's read the write timeout values from the flash 
     * 
     */
    typical_timeout = (*flash->read_query)( flash, 0x1f);
    max_timeout = (*flash->read_query)( flash, 0x23);
    
    if ((typical_timeout == 0 ) || (max_timeout == 0))
    {
      flash->write_timeout = 1000; /* 1ms should be more than enough */
    }
    else
    {
      flash->write_timeout = (1 << typical_timeout) * (1 << max_timeout);
    }
   
    /* Let's read the block erase timeout values from the flash */
    typical_timeout = (*flash->read_query)( flash, 0x21);
    max_timeout = (*flash->read_query)( flash, 0x25);
    
    if ((typical_timeout == 0 ) || (max_timeout == 0))
    {
      flash->erase_timeout = 20000000; /* 20s should be more than enough */
    }
    else
    {
      flash->erase_timeout = (1 << typical_timeout) * (1 << max_timeout) * 1000;
    }
   
    device_size = 0x1 << (*flash->read_query)( flash, 0x27);
  
    flash->dev.number_of_regions = (*flash->read_query)(flash, 0x2c);
    
    if (flash->dev.number_of_regions > ALT_MAX_NUMBER_OF_FLASH_REGIONS)
    {
      ret_code = -ENOMEM;
    }
    else
    {
      for(i=0;i<flash->dev.number_of_regions;i++)
      {
        flash->dev.region_info[i].number_of_blocks =  alt_read_16bit_query_entry( 
                                                            flash,
                                                            (0x2D+i*4));
        flash->dev.region_info[i].number_of_blocks += 1; 
        flash->dev.region_info[i].block_size =  alt_read_16bit_query_entry( flash, 
                                                              (0x2F+i*4));
        flash->dev.region_info[i].block_size *= 256;
        flash->dev.region_info[i].region_size = 
                                    flash->dev.region_info[i].number_of_blocks 
                                    * flash->dev.region_info[i].block_size;
        size += flash->dev.region_info[i].region_size;
      }
       
      if (size != device_size)
      {
        ret_code = -ENODEV;
      }
    }
    
    boot_mode = (*flash->read_query)( flash, flash->primary_address + 0xf);
    
    /* 
     * Intel Flash parts describe the sections in the order they appear
     * for AMD they just put all the small ones first then the bigger ones
     * So if it's a top boot part we have to reverse the order of the sectors
     * so they're in the correct order
     */
    if ((flash->algorithm == CFI_ALG_AMD) && (boot_mode == TOP_BOOT_DEVICE))
    {
      for(i=flash->dev.number_of_regions-1, j=0;
          j<=i;i--,j++)
      {
        swap = flash->dev.region_info[i].region_size;
        flash->dev.region_info[i].region_size =  
                                flash->dev.region_info[j].region_size;
        flash->dev.region_info[j].region_size = swap;

        swap = flash->dev.region_info[i].block_size;
        flash->dev.region_info[i].block_size =  
                                flash->dev.region_info[j].block_size;
        flash->dev.region_info[j].block_size = swap;
 
        swap = flash->dev.region_info[i].number_of_blocks;
        flash->dev.region_info[i].number_of_blocks =  
                                flash->dev.region_info[j].number_of_blocks;
        flash->dev.region_info[j].number_of_blocks = swap;

      } 
    }
    
    for(i=0;i<flash->dev.number_of_regions;i++)
    {
      flash->dev.region_info[i].offset = offset;
      offset += flash->dev.region_info[i].region_size;
    }
    
    (*flash->write_command)(flash->dev.base_addr, 
                            0x55, 
                            READ_ARRAY_MODE);
  }  

  return ret_code;
}


/*
 * alt_read_cfi_width
 * 
 * Work out the width of the device we're talking to and sanity check that we  
 * can read the CFI and the Primary Vendor specific Table
 */
int alt_read_cfi_width(alt_flash_cfi_dev* flash)
{
  int i;
  alt_u8 byte_id[12];
  alt_u16 iface;
  int ret_code = 0;

  /*
  * Check for 8 bit wide flash
  */
  alt_write_flash_command_8bit_device_8bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);

  for(i=0;i<3;i++)
  {
    byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, QUERY_ADDR+i);
  }

  if ((byte_id[0] == 'Q') &&
      (byte_id[1] == 'R') &&
      (byte_id[2] == 'Y'))
  {
    flash->mode_width = 1;
    flash->device_width = 1; 
    iface = IORD_16DIRECT(flash->dev.base_addr, INTERFACE_ADDR);
    iface += 1;
    if (!(iface & 0x1))
    {
      ret_code = -ENODEV;
    }
  }
  else
  {
    /*
    * Check for 8/16 bit in byte wide mode
    */
    alt_write_flash_command_16bit_device_8bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);
    for(i=0;i<6;i++)
    {
      byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, (QUERY_ADDR*2)+i);
    }

    if ((byte_id[0] == 'Q') && 
        (byte_id[1] == 'Q') && 
        (byte_id[2] == 'R') &&
        (byte_id[3] == 'R') && 
        (byte_id[4] == 'Y') && 
        (byte_id[5] == 'Y'))
    {
      flash->mode_width = 1;
      flash->device_width = 2; 
      iface = IORD_16DIRECT(flash->dev.base_addr, INTERFACE_ADDR*2);
      iface += 1;
      if (!(iface & 0x1))
      {
        ret_code = -ENODEV;
      }
    }
    else
    {
      /*
      * Check for 16 bit flash in word mode
      */
      alt_write_flash_command_16bit_device_16bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);
      for(i=0;i<6;i++)
      {
        byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, (QUERY_ADDR*2)+i);
      }

      if ((byte_id[0] == 'Q') && 
          (byte_id[1] == '\0') && 
          (byte_id[2] == 'R') && 
          (byte_id[3] == '\0') && 
          (byte_id[4] == 'Y') && 
          (byte_id[5] == '\0'))
      {
        flash->mode_width = 2;
        flash->device_width = 2; 
        iface = IORD_16DIRECT(flash->dev.base_addr, INTERFACE_ADDR*2);
        iface += 1;
        if (!(iface & 0x2))
        {
          ret_code = -ENODEV;
        }
      }
      else
      {
        /*
        * Check for 32bit wide flash in 32 bit mode
        */
        alt_write_flash_command_32bit_device_32bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);
        for(i=0;i<12;i++)
        {
          byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, (QUERY_ADDR*4)+i);
        }

        if ((byte_id[0] == 'Q') &&
          (byte_id[1] == '\0') && 
          (byte_id[2] == '\0') && 
          (byte_id[3] == '\0') && 
          (byte_id[4] == 'R') && 
          (byte_id[5] == '\0') && 
          (byte_id[6] == '\0') && 
          (byte_id[7] == '\0') && 
          (byte_id[8] == 'Y') && 
          (byte_id[9] == '\0') && 
          (byte_id[10] == '\0') && 
          (byte_id[11] == '\0'))
        {
          flash->mode_width = 4;
          flash->device_width = 4; 
          iface = IORD_32DIRECT(flash->dev.base_addr, INTERFACE_ADDR*4);
          iface += 1;
          if (!(iface & 0x4))
          {
            ret_code = -ENODEV;
          }
        }
        else
        {
          /*
          * Check for 32 bit wide in 16 bit mode
          */
          alt_write_flash_command_32bit_device_16bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);
          for(i=0;i<12;i++)
          {
            byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, (QUERY_ADDR*4)+i);
          }

          if ((byte_id[0] == 'Q') &&
              (byte_id[1] == '\0') &&
              (byte_id[2] == 'Q') &&
              (byte_id[3] == '\0') &&
              (byte_id[4] == 'R') &&
              (byte_id[5] == '\0') &&
              (byte_id[6] == 'R') &&
              (byte_id[7] == '\0') &&
              (byte_id[8] == 'Y') &&
              (byte_id[9] == '\0') &&
              (byte_id[10] == 'Y') &&
              (byte_id[11] == '\0'))
          {
            flash->mode_width = 2;
            flash->device_width = 4; 
            iface = IORD_32DIRECT(flash->dev.base_addr, INTERFACE_ADDR*4);
            iface += 1;
            if (!(iface & 0x4))
            {
              ret_code = -ENODEV;
            }
          }
          else
          {
            /*
            * 32 Bit wide flash in byte mode
            */
            alt_write_flash_command_32bit_device_8bit_mode(flash->dev.base_addr, 0x55, QUERY_MODE);
            for(i=0;i<12;i++)
            {
              byte_id[i] = IORD_8DIRECT(flash->dev.base_addr, (QUERY_ADDR*4)+i);
            }

            if ((byte_id[0] == 'Q') &&
                (byte_id[1] == 'Q') &&
                (byte_id[2] == 'Q') &&
                (byte_id[3] == 'Q') &&
                (byte_id[4] == 'R') && 
                (byte_id[5] == 'R') && 
                (byte_id[6] == 'R') && 
                (byte_id[7] == 'R') && 
                (byte_id[8] == 'Y') && 
                (byte_id[9] == 'Y') && 
                (byte_id[10] == 'Y') && 
                (byte_id[11] == 'Y'))
            {
              flash->mode_width = 1;
              flash->device_width = 4; 
              iface = IORD_32DIRECT(flash->dev.base_addr, INTERFACE_ADDR*4);
              iface += 1;
              if (!(iface & 0x4))
              {
                ret_code = -ENODEV;
              }
            }
          }
        }
      }
    }
  }
  
  return ret_code;
}

/*
 * alt_check_primary_table
 * 
 * Check that the primary Vendor table starts with the 
 * correct pattern
 */
int alt_check_primary_table(alt_flash_cfi_dev* flash)
{
  int i;
  int ret_code = 0;
  alt_u8 primary_query_string[3];
  
  flash->primary_address = alt_read_16bit_query_entry( flash, 
                            PRIMARY_ADDR);
  
  for(i=0;i<3;i++)
  {
    primary_query_string[i] = 
          (*flash->read_query)( flash,(flash->primary_address + i));
  }
    
  if ((primary_query_string[0] != 'P') ||
      (primary_query_string[1] != 'R') ||
      (primary_query_string[2] != 'I'))
  {
    ret_code = -ENODEV;
  }
  
  return ret_code;
}
