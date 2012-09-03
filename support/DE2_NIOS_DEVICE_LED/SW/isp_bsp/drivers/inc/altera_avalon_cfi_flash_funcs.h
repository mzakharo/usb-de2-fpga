#ifndef __ALT_FLASH_CFI_FUNCS_H__
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
* altera_avalon_cfi_flash_funcs.h                                             *
*                                                                             *
* Prototypes to the functions concerned with accessing the CFI table          *
*                                                                             *
* Author PRR                                                                  *
*                                                                             *
******************************************************************************/
#define __ALT_FLASH_CFI_FUNCS_H__

#include "alt_types.h"
alt_u8 alt_read_query_entry_8bit( void* flash_base_addr, int address);
alt_u8 alt_read_query_entry_16bit( void* flash_base_addr, int address);
alt_u8 alt_read_query_entry_32bit( void* flash_base_addr, int address);
int alt_set_flash_width_func( alt_flash_cfi_dev* flash);
int alt_set_flash_algorithm_func( alt_flash_cfi_dev* flash);
void alt_write_value_to_flash(alt_flash_cfi_dev* flash, int offset, const alt_u8* src_addr);
int alt_read_cfi_width(alt_flash_cfi_dev* flash);
int alt_read_cfi_table(alt_flash_cfi_dev* flash);
int alt_flash_program_block(  alt_flash_cfi_dev* flash, const int offset, 
                        const alt_u8* src_addr, const int length,
                        alt_program_word_fn program_word_func);


#endif /* __ALT_FLASH_CFI_FUNCS_H__ */
