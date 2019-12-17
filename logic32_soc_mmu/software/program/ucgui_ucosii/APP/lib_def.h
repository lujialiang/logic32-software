#ifndef _LIB_DEF_H_
#define _LIB_DEF_H_

#define DEF_INT_CPU_NBR_BITS 32uL
#define DEF_INT_CPU_U_MAX_VAL 0xffffffff

/*
*********************************************************************************************************
*                                              DEF_BIT()
*
* Description : Create bit mask with single, specified bit set.
*
* Argument(s) : bit         Bit number of bit to set.
*
* Return(s)   : Bit mask with single, specified bit set.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) (a) 'bit' values that overflow the target CPU &/or compiler environment (e.g. negative 
*                       or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*
*                   (b) To avoid overflowing any target CPU &/or compiler's integer data type, unsigned 
*                       bit constant '1' is suffixed with 'L'ong integer modifier.
*
*                       This may still be insufficient for CPUs &/or compilers that support 'long long' 
*                       integer data types, in which case 'LL' integer modifier should be suffixed.  
*                       However, since almost all 16- & 32-bit CPUs & compilers support 'long' integer 
*                       data types but many may NOT support 'long long' integer data types, only 'long' 
*                       integer data types & modifiers are supported.
*********************************************************************************************************
*/

#define  DEF_BIT(bit)                           (1uL << (bit))


/*
*********************************************************************************************************
*                                           DEF_BIT_MASK()
*
* Description : Shift a bit mask.
*
* Argument(s) : bit_mask    Bit mask to shift.
*
*               bit_shift   Number of bit positions to left-shift bit mask.
*
* Return(s)   : Shifted bit mask.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'bit_shift' values that overflow the target CPU &/or compiler environment (e.g. negative
*                   or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*********************************************************************************************************
*/

#define  DEF_BIT_MASK(bit_mask, bit_shift)             ((bit_mask) << (bit_shift))


/*
*********************************************************************************************************
*                                           DEF_BIT_FIELD()
*
* Description : Create & shift a contiguous bit field.
*
* Argument(s) : bit_field   Number of contiguous bits to set in the bit field.
*
*               bit_shift   Number of bit positions   to left-shift bit field.
*
* Return(s)   : Shifted bit field.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) (a) 'bit_field'/'bit_shift' values that overflow the target CPU &/or compiler 
*                       environment (e.g. negative or greater-than-CPU-data-size values) MAY generate 
*                       compiler warnings &/or errors.
*
*                   (b) To avoid overflowing any target CPU &/or compiler's integer data type, unsigned 
*                       bit constant '1' is suffixed with 'L'ong integer modifier.
*
*                       This may still be insufficient for CPUs &/or compilers that support 'long long' 
*                       integer data types, in which case 'LL' integer modifier should be suffixed.  
*                       However, since almost all 16- & 32-bit CPUs & compilers support 'long' integer 
*                       data types but many may NOT support 'long long' integer data types, only 'long' 
*                       integer data types & modifiers are supported.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD(bit_field, bit_shift)         ((((bit_field) >= DEF_INT_CPU_NBR_BITS) ? (DEF_INT_CPU_U_MAX_VAL)     \
                                                                                             : (DEF_BIT(bit_field) - 1uL))  << (bit_shift))

#endif // _LIB_DEF_H_
