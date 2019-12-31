#ifndef _TMR_h_
#define _TMR_h_

typedef void (*tmr_proc_t)(void*);
typedef unsigned int tmr_id_t;
typedef enum { tmr_sta_run, tmr_sta_stop, tmr_sta_pause } tmr_sta_t;

typedef struct tmr_s {
	tmr_id_t id;
	bool run_once;
	tmr_sta_t sta;
	tmr_proc_t proc;
	unsigned int reload;
	unsigned int counter;
	tmr_s *next;
} tmr_t;

tmr_id_t add_tmr(tmr_proc_t proc, unsigned int reload,bool once);
int del_tmr(tmr_id_t id);
void del_all_tmr(void);
int set_tmr_sta(tmr_id_t id, tmr_sta_t tmr_sta);
void tmr_thread(int arg);

#endif // _TMR_h_
