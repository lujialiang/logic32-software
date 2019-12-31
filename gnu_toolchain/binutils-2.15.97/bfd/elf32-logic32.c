/* logic32 specific support for 32-bit ELF
   Copyright 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/logic32.h"

int    set_logic32_skip_hi16_flag PARAMS ((int));

static bfd_boolean elf32_logic32_check_relocs
  PARAMS ((bfd *, struct bfd_link_info *, asection *,
	   const Elf_Internal_Rela *));
static void elf32_logic32_info_to_howto
  PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
static void elf32_logic32_info_to_howto_rel
  PARAMS ((bfd *, arelent *, Elf_Internal_Rela *));
static bfd_reloc_status_type elf32_logic32_relocate16
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

/*
static bfd_reloc_status_type elf32_logic32_relocate26
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
*/
static bfd_reloc_status_type elf32_logic32_relocate24
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type elf32_logic32_relocate_jmp32
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static bfd_reloc_status_type elf32_logic32_relocate_ldi
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));

static reloc_howto_type *elf32_logic32_reloc_type_lookup
  PARAMS ((bfd *, bfd_reloc_code_real_type));
static bfd_reloc_status_type _bfd_logic32_elf_hi16_reloc
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static reloc_howto_type * logic32_rtype_to_howto
  PARAMS ((unsigned int));


#define USE_REL 1

#define bfd_elf32_bfd_reloc_type_lookup elf32_logic32_reloc_type_lookup
#define elf_info_to_howto               elf32_logic32_info_to_howto
#define elf_info_to_howto_rel           elf32_logic32_info_to_howto_rel
#define elf_backend_check_relocs        elf32_logic32_check_relocs

static reloc_howto_type logic32_elf_howto_table[]=
  {
    /* No relocation.  */
    HOWTO (R_LOGIC32_NONE,            /* type */
	   0,                     /* rightshift */
	   0,                     /* size (0 = byte, 1 = short, 2 = long) */
	   0,                     /* bitsize */
	   FALSE,                 /* pc_relative */
	   0,                     /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   bfd_elf_generic_reloc, /* special_function */
	   "R_LOGIC32_NONE",          /* name */
	   FALSE,                 /* partial_inplace */
	   0,                     /* src_mask */
	   0,                     /* dst_mask */
	   FALSE),                /* pcrel_offset */

    /* 8 bit relocation.  */
    HOWTO (R_LOGIC32_RELOC_8,         /* type */
	   0,                     /* rightshift */
	   0,                     /* size (0 = byte, 1 = short, 2 = long) */
	   8,                     /* bitsize */
	   FALSE,                 /* pc_relative */
	   0,                     /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   bfd_elf_generic_reloc, /* special_function */
	   "R_LOGIC32_RELOC_8",       /* name */
	   TRUE,                  /* partial_inplace */
	   0xff,                  /* src_mask */
	   0xff,                  /* dst_mask */
	   FALSE),                /* pcrel_offset */

    /* 16 bit relocation.  */
    HOWTO (R_LOGIC32_RELOC_16,        /* type */
	   0,                     /* rightshift */
	   1,                     /* size (0 = byte, 1 = short, 2 = long) */
	   16,                    /* bitsize */
	   FALSE,                 /* pc_relative */
	   0,                     /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   bfd_elf_generic_reloc, /* special_function */
	   "R_LOGIC32_RELOC_16",      /* name */
	   TRUE,                  /* partial_inplace */
	   0xffff,                /* src_mask */
	   0xffff,                /* dst_mask */
	   FALSE),                /* pcrel_offset */

    /* 32 bit relocation.  */
    HOWTO (R_LOGIC32_RELOC_32,        /* type */
	   0,                     /* rightshift */
	   2,                     /* size (0 = byte, 1 = short, 2 = long) */
	   32,                    /* bitsize */
	   FALSE,                 /* pc_relative */
	   0,                     /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   bfd_elf_generic_reloc, /* special_function */
	   "R_LOGIC32_RELOC_32",      /* name */
	   TRUE,                  /* partial_inplace */
	   0xffffffff,            /* src_mask */
	   0xffffffff,            /* dst_mask */
	   FALSE),                /* pcrel_offset */

    /* GNU extension to record C++ vtable hierarchy */
    HOWTO (R_LOGIC32_GNU_VTINHERIT,   /* type */
	   0,			  /* rightshift */
	   2,			  /* size (0 = byte, 1 = short, 2 = long) */
	   0,			  /* bitsize */
	   FALSE,		  /* pc_relative */
	   0,			  /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   NULL,		  /* special_function */
	   "R_LOGIC32_GNU_VTINHERIT", /* name */
	   FALSE,		  /* partial_inplace */
	   0,			  /* src_mask */
	   0,			  /* dst_mask */
	   FALSE),		  /* pcrel_offset */

    /* GNU extension to record C++ vtable member usage */
    HOWTO (R_LOGIC32_GNU_VTENTRY,     /* type */
	   0,			  /* rightshift */
	   2,			  /* size (0 = byte, 1 = short, 2 = long) */
	   0,			  /* bitsize */
	   FALSE,		  /* pc_relative */
	   0,			  /* bitpos */
	   complain_overflow_dont,/* complain_on_overflow */
	   _bfd_elf_rel_vtable_reloc_fn,/* special_function */
	   "R_LOGIC32_GNU_VTENTRY",	  /* name */
	   FALSE,		  /* partial_inplace */
	   0,			  /* src_mask */
	   0,			  /* dst_mask */
	   FALSE)		  /* pcrel_offset */
  };

/* 16 bit offset for pc-relative branches.  */
static reloc_howto_type elf_logic32_gnu_rel16_s2 =
HOWTO (R_LOGIC32_RELOC_16_PCREL,  /* type */
       0,                     /* rightshift */
       1,                     /* size (0 = byte, 1 = short, 2 = long) */
       16,                    /* bitsize */
       TRUE,                  /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_signed, /* complain_on_overflow */
       elf32_logic32_relocate16,  /* special_function */
       "R_LOGIC32_RELOC_16_PCREL",/* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       TRUE);                 /* pcrel_offset */

#if 0
/* 26 bit offset for pc-relative branches.  */
static reloc_howto_type elf_logic32_gnu_rel26_s2 =
HOWTO (R_LOGIC32_RELOC_26_PCREL,  /* type */
       0,                     /* rightshift */
       2,                     /* size (0 = byte, 1 = short, 2 = long) */
       26,                    /* bitsize */
       TRUE,                  /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont,/* complain_on_overflow */
       elf32_logic32_relocate26,  /* special_function */
       "R_LOGIC32_RELOC_26_PCREL",/* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       TRUE);                 /* pcrel_offset */
#endif

/* 24 bit offset for pc-relative branches.  */
static reloc_howto_type elf_logic32_gnu_rel24_s2 =
HOWTO (R_LOGIC32_RELOC_24_PCREL,  /* type */
       0,                     /* rightshift */
       2,                     /* size (0 = byte, 1 = short, 2 = long) */
       24,                    /* bitsize */
       TRUE,                  /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont,/* complain_on_overflow */
       elf32_logic32_relocate24,  /* special_function */
       "R_LOGIC32_RELOC_24_PCREL",/* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       TRUE);                 /* pcrel_offset */

/* 32 bit offset for absolute jump.  */
static reloc_howto_type elf_logic32_gnu_jmp32_s2 =
HOWTO (R_LOGIC32_RELOC_32_JMP,  /* type */
       0,                     /* rightshift */
       2,                     /* size (0 = byte, 1 = short, 2 = long) */
       32,                    /* bitsize */
       FALSE,                  /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont,/* complain_on_overflow */
       elf32_logic32_relocate_jmp32,  /* special_function */
       "R_LOGIC32_RELOC_32_JMP",/* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       FALSE);                 /* pcrel_offset */

/* ldi  */
static reloc_howto_type elf_logic32_gnu_ldi_s2 =
HOWTO (R_LOGIC32_RELOC_LDI,  /* type */
       0,                     /* rightshift */
       2,                     /* size (0 = byte, 1 = short, 2 = long) */
       32,                    /* bitsize */
       FALSE,                  /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont,/* complain_on_overflow */
       elf32_logic32_relocate_ldi,  /* special_function */
       "R_LOGIC32_RELOC_LDI", /* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       FALSE);                 /* pcrel_offset */

/* High 16 bits of symbol value.  */
static reloc_howto_type elf_logic32_reloc_16_hi =
HOWTO (R_LOGIC32_RELOC_16_HI,     /* type */
       16,                    /* rightshift */
       2,                     /* size (0 = byte, 1 = short, 2 = long) */
       32,                    /* bitsize */
       FALSE,                 /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont, /* complain_on_overflow */
       _bfd_logic32_elf_hi16_reloc,/* special_function */
       "R_LOGIC32_RELOC_16_HI",   /* name */
       TRUE,                  /* partial_inplace */
       0xFFFF,                /* src_mask */
       0xffff,                /* dst_mask */
       FALSE);                /* pcrel_offset */

  /* Low 16 bits of symbol value.  */
static reloc_howto_type elf_logic32_reloc_16_lo =
HOWTO (R_LOGIC32_RELOC_16_LO,     /* type */
       0,                     /* rightshift */
       1,                     /* size (0 = byte, 1 = short, 2 = long) */
       16,                    /* bitsize */
       FALSE,                 /* pc_relative */
       0,                     /* bitpos */
       complain_overflow_dont,/* complain_on_overflow */
       bfd_elf_generic_reloc, /* special_function */
       "R_LOGIC32_RELOC_16_LO",   /* name */
       TRUE,                  /* partial_inplace */
       0xffff,                /* src_mask */
       0xffff,                /* dst_mask */
       FALSE);                /* pcrel_offset */


/* The gas default behavior is not to preform the %hi modifier so that the
   GNU assembler can have the lower 16 bits offset placed in the insn, BUT
   we do like the gas to indicate it is %hi reloc type so when we in the link
   loader phase we can have the corrected hi16 vale replace the buggous lo16
   value that was placed there by gas.  */

static int skip_logic32_elf_hi16_reloc = 0;

int
set_logic32_skip_hi16_flag (flag)
     int flag;
{
  skip_logic32_elf_hi16_reloc = flag;
  return flag;
}

static bfd_reloc_status_type
_bfd_logic32_elf_hi16_reloc (abfd, reloc_entry, symbol, data,
			 input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message;
{
  bfd_reloc_status_type ret;
  bfd_vma relocation;

  /* If the skip flag is set then we simply do the generic relocating, this
     is more of a hack for logic32 gas/gld, so we do not need to do the %hi/%lo
     fixup like mips gld did.   */
  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                          input_section, output_bfd, error_message);

  /* If we're relocating, and this an external symbol, we don't want
     to change anything.  */
  if (output_bfd != (bfd *) NULL
      && (symbol->flags & BSF_SECTION_SYM) == 0
      && reloc_entry->addend == 0)
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }

  ret = bfd_reloc_ok;

  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    ret = bfd_reloc_undefined;

  relocation = (bfd_is_com_section (symbol->section)) ? 0 : symbol->value;
  relocation += symbol->section->output_section->vma;
  relocation += symbol->section->output_offset;
  relocation += reloc_entry->addend;
  relocation += bfd_get_16 (abfd, (bfd_byte *)data + reloc_entry->address);

  if (reloc_entry->address > bfd_get_section_limit (abfd, input_section))
    return bfd_reloc_outofrange;

  bfd_put_16 (abfd, (short)((relocation >> 16) & 0xFFFF),
              (bfd_byte *)data + reloc_entry->address);

  return ret;
}

/* ELF relocs are against symbols.  If we are producing relocatable
   output, and the reloc is against an external symbol, and nothing
   has given us any additional addend, the resulting reloc will also
   be against the same symbol.  In such a case, we don't want to
   change anything about the way the reloc is handled, since it will
   all be done at final link time.  Rather than put special case code
   into bfd_perform_relocation, all the reloc types use this howto
   function.  It just short circuits the reloc if producing
   relocatable output against an external symbol.  */

static bfd_reloc_status_type
elf32_logic32_relocate16  (abfd, reloc_entry, symbol, data,
                       input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  unsigned long insn, vallo, allignment;
  int           val;

  /* HACK: I think this first condition is necessary when producing
     relocatable output.  After the end of HACK, the code is identical
     to bfd_elf_generic_reloc().  I would _guess_ the first change
     belongs there rather than here.  martindo 1998-10-23.  */

  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                                 input_section, output_bfd, error_message);

  /* Check undefined section and undefined symbols  */
  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    return bfd_reloc_undefined;

  /* Can not support a long jump to sections other then .text   */
  if (strcmp (input_section->name, symbol->section->output_section->name) != 0)
    {
      fprintf (stderr,
	       "BFD Link Error: branch (PC rel16) to section (%s) not supported\n",
	       symbol->section->output_section->name);
      return bfd_reloc_undefined;
    }

  insn  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address);
  allignment = 1 << (input_section->output_section->alignment_power - 1);
  vallo = insn & 0x0000FFFF;

  if (vallo & 0x8000)
    vallo = ~(vallo | 0xFFFF0000) + 1;

  /* vallo points to the vma of next instruction.  */
  vallo += (((unsigned long)(input_section->output_section->vma +
                           input_section->output_offset) +
            allignment) & ~allignment);

  /* val is the displacement (PC relative to next instruction).  */
  val =  (symbol->section->output_offset +
	  symbol->section->output_section->vma +
	  symbol->value) - vallo;

  if (abs ((int) val) > 0x00007FFF)
    return bfd_reloc_outofrange;

  insn  = (insn & 0xFFFF0000) | (val & 0x0000FFFF);

  bfd_put_32 (abfd, insn,
              (bfd_byte *) data + reloc_entry->address);

  return bfd_reloc_ok;
}

#if 0
static bfd_reloc_status_type
elf32_logic32_relocate26  (abfd, reloc_entry, symbol, data,
                       input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  unsigned long insn, vallo, allignment;
  int           val;

  /* HACK: I think this first condition is necessary when producing
     relocatable output.  After the end of HACK, the code is identical
     to bfd_elf_generic_reloc().  I would _guess_ the first change
     belongs there rather than here.  martindo 1998-10-23.  */

  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                                 input_section, output_bfd, error_message);

  /* Check undefined section and undefined symbols.  */
  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    return bfd_reloc_undefined;

  /* Can not support a long jump to sections other then .text   */
  if (strcmp (input_section->name, symbol->section->output_section->name) != 0)
    {
      fprintf (stderr,
	       "BFD Link Error: jump (PC rel26) to section (%s) not supported\n",
	       symbol->section->output_section->name);
      return bfd_reloc_undefined;
    }

  insn  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address);
  allignment = 1 << (input_section->output_section->alignment_power - 1);
  vallo = insn & 0x03FFFFFF;

  if (vallo & 0x03000000)
    vallo = ~(vallo | 0xFC000000) + 1;

  /* vallo is the vma for the next instruction.  */
  vallo += (((unsigned long) (input_section->output_section->vma +
			      input_section->output_offset) +
	     allignment) & ~allignment);

  /* val is the displacement (PC relative to next instruction).  */
  val = (symbol->section->output_offset +
	 symbol->section->output_section->vma + symbol->value)
    - vallo;

  if (abs ((int) val) > 0x01FFFFFF)
    return bfd_reloc_outofrange;

  insn  = (insn & 0xFC000000) | (val & 0x03FFFFFF);
  bfd_put_32 (abfd, insn,
              (bfd_byte *) data + reloc_entry->address);

  return bfd_reloc_ok;
}
#endif

static bfd_reloc_status_type
elf32_logic32_relocate24  (abfd, reloc_entry, symbol, data,
                       input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  unsigned long insn, vallo, allignment;
  int           val;

  /* HACK: I think this first condition is necessary when producing
     relocatable output.  After the end of HACK, the code is identical
     to bfd_elf_generic_reloc().  I would _guess_ the first change
     belongs there rather than here.  martindo 1998-10-23.  */

  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                                 input_section, output_bfd, error_message);

  /* Check undefined section and undefined symbols.  */
  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    return bfd_reloc_undefined;

  /* Can not support a long jump to sections other then .text   */
  if (strcmp (input_section->name, symbol->section->output_section->name) != 0)
    {
      fprintf (stderr,
	       "BFD Link Error: jump (PC rel24) to section (%s) not supported\n",
	       symbol->section->output_section->name);
      return bfd_reloc_undefined;
    }

  insn  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address);
printf("## inst: 0x%08x\n", insn);
  allignment = 1 << (input_section->output_section->alignment_power - 1);
  vallo = insn & 0x00FFFFFF;

  if (vallo & 0x00800000)
    vallo = ~(vallo | 0xFF000000) + 1;

  /* vallo is the vma for the next instruction.  */
  vallo += (((unsigned long) (input_section->output_section->vma +
			      input_section->output_offset) +
	     allignment) & ~allignment);

  /* val is the displacement (PC relative to next instruction).  */
  val = (symbol->section->output_offset +
	 symbol->section->output_section->vma + symbol->value)
    - vallo;

printf("## val: 0x%08x\n", val);

  if (abs ((int) val) > 0x007FFFFF)
    return bfd_reloc_outofrange;

  insn  = (insn & 0xFF000000) | (val & 0x00FFFFFF);
printf("## inst1: 0x%08x\n", insn);
  bfd_put_32 (abfd, insn,
              (bfd_byte *) data + reloc_entry->address);

  return bfd_reloc_ok;
}

static bfd_reloc_status_type
elf32_logic32_relocate_jmp32  (abfd, reloc_entry, symbol, data,
                       input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  unsigned long insn;
/*
, vallo, allignment;
*/
  int           val;
  unsigned long insn_offset;

  /* HACK: I think this first condition is necessary when producing
     relocatable output.  After the end of HACK, the code is identical
     to bfd_elf_generic_reloc().  I would _guess_ the first change
     belongs there rather than here.  martindo 1998-10-23.  */

  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                                 input_section, output_bfd, error_message);

  /* Check undefined section and undefined symbols.  */
  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    return bfd_reloc_undefined;

  /* Can not support a long jump to sections other then .text   */
#if 0
  if (strcmp (input_section->name, symbol->section->output_section->name) != 0)
    {
      fprintf (stderr,
	       "BFD Link Error: jump (PC 32) to section (%s) not supported\n",
	       symbol->section->output_section->name);
      return bfd_reloc_undefined;
    }
#else
#endif

  insn  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address);
  val = (symbol->section->output_offset +
	 symbol->section->output_section->vma + symbol->value);
  insn_offset = ((insn & 0x3fffffff)<<2); 
  val += insn_offset;
  if (val & 3 ) {
	fprintf (stderr, "must allignment by 4\n");
	return bfd_reloc_notsupported;
  }

/*
 printf("insn: 0x%08x, insn offset: 0x%08x, symbol name: %s, output offset: 0x%08x, vma: 0x%08x, symbol value: 0x%08x, val: 0x%08x\n", insn, insn_offset, symbol->name, symbol->section->output_offset, symbol->section->output_section->vma ,symbol->value, val);
*/

  val >>= 2;

  if (abs ((int) val) > 0x1FFFFFFF)
    return bfd_reloc_outofrange;

  insn  = (insn & 0xfc000000) | (val & 0x3FFFFFFF);

  bfd_put_32 (abfd, insn,
              (bfd_byte *) data + reloc_entry->address);

  return bfd_reloc_ok;
}

static bfd_reloc_status_type
elf32_logic32_relocate_ldi  (abfd, reloc_entry, symbol, data,
                       input_section, output_bfd, error_message)
     bfd *abfd;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  unsigned long insn1, insn2, allignment;
  unsigned long tmp;
  int           val;

  /* HACK: I think this first condition is necessary when producing
     relocatable output.  After the end of HACK, the code is identical
     to bfd_elf_generic_reloc().  I would _guess_ the first change
     belongs there rather than here.  martindo 1998-10-23.  */

  if (skip_logic32_elf_hi16_reloc)
    return bfd_elf_generic_reloc (abfd, reloc_entry, symbol, data,
                                 input_section, output_bfd, error_message);

  /* Check undefined section and undefined symbols.  */
  if (bfd_is_und_section (symbol->section)
      && output_bfd == (bfd *) NULL)
    return bfd_reloc_undefined;

  insn1  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address);
  insn2  = bfd_get_32 (abfd, (bfd_byte *)data + reloc_entry->address + 4);
  tmp = ((insn1&0xffff)<<16)|(insn2&0xffff);
  val = (symbol->section->output_offset +
	 symbol->section->output_section->vma + symbol->value);
  tmp += val;
	//printf("val: 0x%08x, inst: 0x%08x, inst2: 0x%08x, tmp: 0x%08x\n", val, insn1, insn2, tmp);

  insn1 = (insn1 & 0xffff0000) | ((tmp>>16) & 0xFFFF);
  insn2 = (insn2 & 0xffff0000) | (tmp & 0xFFFF);
  bfd_put_32 (abfd, insn1,
              (bfd_byte *) data + reloc_entry->address);
  bfd_put_32 (abfd, insn2,
              (bfd_byte *) data + reloc_entry->address+4);

  return bfd_reloc_ok;
}

/* A mapping from BFD reloc types to LOGIC32 ELF reloc types.
   Stolen from elf32-mips.c.

   More about this table - for logic32 elf relocation we do not really
   need this table, if we have a rtype defined in this table will
   caused tc_gen_relocate confused and die on us, but if we remove
   this table it will caused more problem, so for now simple solution
   is to remove those entries which may cause problem.  */
struct elf_reloc_map
{
  bfd_reloc_code_real_type bfd_reloc_val;
  enum elf_logic32_reloc_type elf_reloc_val;
};

static const struct elf_reloc_map logic32_reloc_map[] =
  {
    { BFD_RELOC_NONE,           R_LOGIC32_NONE },
    { BFD_RELOC_16,             R_LOGIC32_RELOC_16 },
    { BFD_RELOC_32,             R_LOGIC32_RELOC_32 },
    { BFD_RELOC_LOGIC32_HI16_S,     R_LOGIC32_RELOC_16_HI },
    { BFD_RELOC_LOGIC32_LO16,       R_LOGIC32_RELOC_16_LO },
    { BFD_RELOC_VTABLE_INHERIT,	R_LOGIC32_GNU_VTINHERIT },
    { BFD_RELOC_VTABLE_ENTRY,	R_LOGIC32_GNU_VTENTRY }
  };


/* Look through the relocs for a section during the first phase.
   Since we don't do .gots or .plts, we just need to consider the
   virtual table relocs for gc.  */

static bfd_boolean
elf32_logic32_check_relocs (abfd, info, sec, relocs)
     bfd *abfd;
     struct bfd_link_info *info;
     asection *sec;
     const Elf_Internal_Rela *relocs;
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes, **sym_hashes_end;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;

  if (info->relocatable)
    return TRUE;

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (abfd);
  sym_hashes_end = sym_hashes + symtab_hdr->sh_size / sizeof (Elf32_External_Sym);
  if (!elf_bad_symtab (abfd))
    sym_hashes_end -= symtab_hdr->sh_info;

  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      struct elf_link_hash_entry *h;
      unsigned long r_symndx;

      r_symndx = ELF32_R_SYM (rel->r_info);
      if (r_symndx < symtab_hdr->sh_info)
        h = NULL;
      else
        h = sym_hashes[r_symndx - symtab_hdr->sh_info];

      switch (ELF32_R_TYPE (rel->r_info))
        {
        /* This relocation describes the C++ object vtable hierarchy.
           Reconstruct it for later use during GC.  */
        case R_LOGIC32_GNU_VTINHERIT:
          if (!bfd_elf_gc_record_vtinherit (abfd, sec, h, rel->r_offset))
            return FALSE;
          break;

        /* This relocation describes which C++ vtable entries are actually
           used.  Record for later use during GC.  */
        case R_LOGIC32_GNU_VTENTRY:
          if (!bfd_elf_gc_record_vtentry (abfd, sec, h, rel->r_addend))
            return FALSE;
          break;
        }
    }

  return TRUE;
}

/* Given a BFD reloc type, return a howto structure.  */

static reloc_howto_type *
elf32_logic32_reloc_type_lookup (abfd, code)
     bfd *abfd ATTRIBUTE_UNUSED;
     bfd_reloc_code_real_type code;
{
  unsigned int i;

  for (i = 0; i < sizeof (logic32_reloc_map) / sizeof (struct elf_reloc_map); i++)
    if (logic32_reloc_map[i].bfd_reloc_val == code)
      return &logic32_elf_howto_table[(int) logic32_reloc_map[i].elf_reloc_val];

  switch (code)
    {
    default:
      bfd_set_error (bfd_error_bad_value);
      return NULL;
    case BFD_RELOC_16_PCREL_S2:
      return &elf_logic32_gnu_rel16_s2;
#if 0
    case BFD_RELOC_LOGIC32_JMP26:
      return &elf_logic32_gnu_rel26_s2;
#endif
    case BFD_RELOC_LOGIC32_JMP24:
      return &elf_logic32_gnu_rel24_s2;
    case BFD_RELOC_LOGIC32_JMP32:
      return &elf_logic32_gnu_jmp32_s2;
    case BFD_RELOC_LOGIC32_LDI:
      return &elf_logic32_gnu_ldi_s2;
    case BFD_RELOC_HI16_S:
      return &elf_logic32_reloc_16_hi;
    case BFD_RELOC_LO16:
      return &elf_logic32_reloc_16_lo;
    }
}

static reloc_howto_type *
logic32_rtype_to_howto (r_type)
     unsigned int r_type;
{
  switch (r_type)
    {
    case R_LOGIC32_RELOC_16_PCREL:
      return & elf_logic32_gnu_rel16_s2;
      break;
#if 0
    case R_LOGIC32_RELOC_26_PCREL:
      return & elf_logic32_gnu_rel26_s2;
      break;
#endif
    case R_LOGIC32_RELOC_24_PCREL:
      return & elf_logic32_gnu_rel24_s2;
      break;
    case R_LOGIC32_RELOC_32_JMP:
      return & elf_logic32_gnu_jmp32_s2;
      break;
    case R_LOGIC32_RELOC_LDI:
      return & elf_logic32_gnu_ldi_s2;
      break;
    case R_LOGIC32_RELOC_16_HI:
      return & elf_logic32_reloc_16_hi;
      break;
    case R_LOGIC32_RELOC_16_LO:
      return & elf_logic32_reloc_16_lo;
      break;

    default:
      BFD_ASSERT (r_type < (unsigned int) R_LOGIC32_max);
      return & logic32_elf_howto_table[r_type];
      break;
    }
}

static void
elf32_logic32_info_to_howto (abfd, cache_ptr, dst)
     bfd * abfd ATTRIBUTE_UNUSED;
     arelent * cache_ptr ATTRIBUTE_UNUSED;
     Elf_Internal_Rela * dst ATTRIBUTE_UNUSED;
{
  abort ();
}

static void
elf32_logic32_info_to_howto_rel (abfd, cache_ptr, dst)
     bfd *abfd ATTRIBUTE_UNUSED;
     arelent *cache_ptr;
     Elf_Internal_Rela *dst;
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  cache_ptr->howto = logic32_rtype_to_howto (r_type);
  return;
}

#define TARGET_LITTLE_SYM          bfd_elf32_logic32_vec
#define TARGET_LITTLE_NAME         "elf32-logic32"
#define ELF_ARCH                bfd_arch_logic32
#define ELF_MACHINE_CODE        EM_LOGIC32
#define ELF_MAXPAGESIZE         1 /* FIXME: This number is wrong,  It should be the page size in bytes.  */

#include "elf32-target.h"
