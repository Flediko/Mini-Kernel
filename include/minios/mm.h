#ifndef _MM_H
#define _MM_H

#define TOTAL_MEMORY_KB 32768  /* 32 MB in KB */
#define PAGE_SIZE_KB 4

extern void setup_paging(void);
extern void load_page_directory(unsigned long *);
extern void enable_paging(void);
extern void get_memory_info(unsigned long *total_kb, unsigned long *used_kb, unsigned long *free_kb);

#endif
