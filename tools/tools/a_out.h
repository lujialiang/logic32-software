#ifndef _A_OUT_H_INCLUDE
#define _A_OUT_H_INCLUDE

#include <stdint.h>

struct	exec {	/* a.out header */
	uint32_t	a_magic;	/* magic number */
	uint32_t	a_text;		/* size of text segment */
	uint32_t	a_data;		/* size of initialized data */
	uint32_t	a_bss;		/* size of unitialized data */
	uint32_t	a_syms;		/* size of symbol table */
	uint32_t	a_entry;	/* entry point */
	uint32_t	a_trsize;
	uint32_t	a_drsize;
	uint32_t	a_strs;
};

#define	A_MAGIC1	0407		/* normal */
#define	A_MAGIC2	0410		/* read-only text */
#define	A_MAGIC3	0411		/* separated I&D */
#define	A_MAGIC4	0405		/* overlay */

struct  relocation_info {
	uint32_t	address;
	uint32_t	index :24,
	pcrel_flag	:1,
	#if 0
	length		:2,
	extern_flag	:1,
	space		:4;
	#else
	length		:3,
	extern_flag	:1,
	space		:3;
	#endif
};

struct	nlist {	/* symbol table entry */
	union {
		char *n_name;
		struct nlist *n_next;
		uint32_t n_strx;
	};
	uint8_t		n_type;		/* type flag */
	int8_t		n_other;
	int16_t		n_desc;
	uint32_t	n_value;	/* value */
};

/* values for type flag */
#define	N_UNDF	0x00	/* undefined */
#define	N_ABS	0x02	/* absolute */
#define	N_TEXT	0x04	/* text symbol */
#define	N_DATA	0x06	/* data symbol */
#define	N_BSS	0x08	/* bss symbol */

#define	N_EXT	0x01	/* external bit, or'ed in */

#define	FORMAT	"%08x"	/* to print a value */

#endif // _A_OUT_H_INCLUDE
