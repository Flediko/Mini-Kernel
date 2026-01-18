#define __MINIOS_LIBC 1
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

/* kernel includes */
#include <time.h>
#include <asm/io.h>
#include <asm/system.h>
#include <minios/tty.h>
#include <minios/head.h>
#include <minios/sched.h>
#include <minios/kernel.h>
#include <minios/fs.h>

static char buf[1024];

extern int vsprintf();
static void init();

void main()
{	
	
	tty_init();
	trap_init();
	sched_init();
	fs_init();
	sti();

	init();

	for (;;);
}

static size_t printf(const char *fmt, ...)
{
	va_list args;
        size_t i;

        va_start(args, fmt);
        i = vsprintf(buf, fmt, args);
        va_end(args);
        puts(buf);
        return i;
}

static void init()
{
	printf("Initializing ...\n");
	timer_wait(50);
	printf("\n[OK]");
	timer_wait(10);
	clrscr();

	printf("WELCOME TO MINIOS\n");
	printf("\nMinios %s is an educational operating system for x86 machines \n", VERSION);
	shell();
}
