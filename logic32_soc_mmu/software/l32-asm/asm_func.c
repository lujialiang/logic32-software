#include "asm.h"

static const struct keyw_s kwtab[] = {
	".equ", sym_equ,
	".org", sym_org,
	".global", sym_global,
	".globl", sym_global,
	".align", sym_align,
	".include", sym_include,
	".text", sym_text,
	".data", sym_data,
	".rdata", sym_data,	// read only data
	".bss",  sym_bss,

	".byte", sym_byte,
	".half", sym_half,
	".word", sym_word,
	".ubyte", sym_ubyte,
	".uhalf", sym_uhalf,
	".uword", sym_uword,

	".lcomm", sym_lcomm,
	".comm",  sym_comm,
	".ascii", sym_ascii,
	".asciz", sym_asciz,
	".end",   sym_end,
	".extern", sym_extern,
	".space", sym_space,
	NULL, 0,
};

struct keyw_s *is_keyword(void)
{
	struct keyw_s *pkw = NULL;
	pkw = (struct keyw_s*)kwtab;
	while(pkw->name) {
		if( match(pkw->name) ) {
			return pkw;
		}
		pkw++;
	}
	return NULL;
}

optab_t *is_opcode(void)
{
	optab_t *p = NULL;
	p = optab;
	while(p->opname) {
		if( match(p->opname) ) {
			return p;
		}
		p++;
	}
	return NULL;
}

int is_name(void)
{
	char *p = token_buf;
	if( (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p == '_') ) {
		p++;
		for(;*p;p++) {
			if( *p >= 'a' && *p <= 'z') {
				continue;
			}
			else if( *p >= 'A' && *p <= 'Z') {
				continue;
			}
			else if( *p >= '0' && *p <= '9') {
				continue;
			}
			else if( *p == '_' ) {
				continue;
			}
			else if( *p == '.' ) {
				continue;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int is_sp(void)
{
	char *sym = token_buf;
	if( sym[2] != '\0' ) {
		return -1;
	}
	if( sym[0] == 's' && sym[1] == 'p' )
	{
		return 32;
	}
	if( sym[0] == 'S' && sym[1] == 'P' )
	{
		return 32;
	}
	return -1;
}

int is_gpr(void)
{
	char *sym = token_buf;
	if( sym[0] == '$' ) {
		if(sym[2] == '\0') {
			if( sym[1] >= '0' && sym[1] <= '9' ) { // $0~$9
				return (sym[1] - '0');
			}
			return -1;
		}
		else if( sym[3] == '\0' ) {
			if( sym[1] == 's' && sym[2] == 'p' || sym[2] == 'S' && sym[2] == 'P') { // $sp
				return SP_REG_IDX;
			}
			if( sym[1] == '1' || sym[1] == '2' ) {
				if( sym[2] >= '0' && sym[2] <= '9' ) {	// $10~$29
					return ((sym[1]-'0')*10 + (sym[2] - '0'));
				}
				return -1;
			}
			else if( sym[1] == '3' ) {
				if( sym[2] >= '0' && sym[2] <= '1' ) { // $30~$31
					return (30 + (sym[2] - '0'));
				}
				return -1;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}

int is_cpr(void)
{
	char *sym = token_buf;
	if( sym[0] == '$' ) {
		if( strcmp(&sym[1], "psw") == 0 || strcmp(&sym[1], "PSW") == 0 ) {
			return PSW_REG_IDX;
		}
		if( sym[1] != 'c' && sym[1] != 'C' ) {
			return -1;
		}
		sym++;
		if(sym[2] == '\0') {
			if( sym[1] >= '0' && sym[1] <= '9' ) { // $0~$9
				return (sym[1] - '0');
			}
			return -1;
		}
		else if( sym[3] == '\0' ) {
			if( sym[1] == '1' || sym[1] == '2' ) {
				if( sym[2] >= '0' && sym[2] <= '9' ) {	// $10~$29
					return ((sym[1]-'0')*10 + (sym[2] - '0'));
				}
				return -1;
			}
			else if( sym[1] == '3' ) {
				if( sym[2] >= '0' && sym[2] <= '1' ) { // $30~$31
					return (30 + (sym[2] - '0'));
				}
				return -1;
			}
			return -1;
		}
		return -1;
	}
	return -1;
}
