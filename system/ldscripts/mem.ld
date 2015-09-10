/*
 * Memory Spaces Definitions.
 *
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

MEMORY
{
/* from atmel
 *  rom (rx)    : ORIGIN = 0x00080000, LENGTH = 0x00080000 
 * sram0 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00010000 
 * sram1 (rwx) : ORIGIN = 0x20080000, LENGTH = 0x00008000 
 * ram (rwx)   : ORIGIN = 0x20070000, LENGTH = 0x00018000 
*/
  FLASH (rx)    : ORIGIN = 0x00080000, LENGTH = 0x00080000 /* Flash, 512K */
  RAM (rwx)   : ORIGIN = 0x20070000, LENGTH = 0x00018000 /* sram, 96K */

/*  FLASH (rx) : ORIGIN = 0x00080000, LENGTH = 512K
  RAM (xrw) : ORIGIN = 0x20070000, LENGTH = 96K
*/
  /*
   * Optional sections; define the origin and length to match
   * the the specific requirements of your hardware. The zero
   * length prevents inadvertent allocation.
   */
  CCMRAM (xrw) : ORIGIN = 0x10000000, LENGTH = 0
  FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
}

/*
 * For external ram use something like:
 *  RAM (xrw) : ORIGIN = 0x64000000, LENGTH = 2048K
 *
 * For special RAM areas use something like:
 *  MEMORY_ARRAY (xrw)  : ORIGIN = 0x20002000, LENGTH = 32
 */
