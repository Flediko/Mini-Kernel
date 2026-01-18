#include <string.h>
#include <time.h>
#include <asm/io.h>
#include <asm/system.h>
#include <minios/tty.h>
#include <minios/kernel.h>
#include <minios/mm.h>
#include <minios/fs.h>

extern void timer_wait(size_t ticks);
extern struct minios_file file_table[];

void prompt(void)
{
printk("\nminios $ ");
}

void shell(void)
{
prompt();
}

static void reboot(void)
{
unsigned char good = 0x02;
while (good & 0x02)
good = inb(0x64);
outb(0x64, 0xFE);
hlt();
}

static int atoi(const char *str)
{
int result = 0;
int sign = 1;
int i = 0;

while (str[i] == ' ' || str[i] == '\t')
i++;

if (str[i] == '-') {
sign = -1;
i++;
} else if (str[i] == '+') {
i++;
}

while (str[i] >= '0' && str[i] <= '9') {
result = result * 10 + (str[i] - '0');
i++;
}

return sign * result;
}

static void cmd_touch(char *filename)
{
int result;
if (filename == (char*)0 || filename[0] == '\0') {
printk("\nUsage: touch <filename>\n");
return;
}
result = fs_create(filename);
if (result == 0) {
printk("\nFile '%s' created successfully\n", filename);
} else if (result == -1) {
printk("\nError: File '%s' already exists\n", filename);
} else {
printk("\nError: No free space\n");
}
}

static void cmd_rm(char *filename)
{
int result;
if (filename == (char*)0 || filename[0] == '\0') {
printk("\nUsage: rm <filename>\n");
return;
}
result = fs_delete(filename);
if (result == 0) {
printk("\nFile '%s' deleted\n", filename);
} else {
printk("\nError: File not found\n");
}
}

static void cmd_ls(void)
{
int i, count = 0;
printk("\nFiles:\n");
for (i = 0; i < 16; i++) {
if (file_table[i].used) {
printk("%s (%d bytes)\n", file_table[i].name, file_table[i].size);
count++;
}
}
if (count == 0) printk("(empty)\n");
printk("Total: %d files\n", count);
}

static void cmd_cat(char *filename)
{
char buffer[2048];
int bytes_read, i;
if (filename == (char*)0 || filename[0] == '\0') {
printk("\nUsage: cat <filename>\n");
return;
}
bytes_read = fs_read(filename, buffer, 2048);
if (bytes_read < 0) {
printk("\nError: File not found\n");
return;
}
printk("\n");
for (i = 0; i < bytes_read; i++) printk("%c", buffer[i]);
printk("\n");
}

static void cmd_echo_to_file(char *args)
{
char fname[32];
int i = 0, j = 0, text_len;
char *text_start;
while (args[i] && args[i] != '>') i++;
if (args[i] != '>') {
printk("\nUsage: echo text > filename\n");
return;
}
text_start = args;
text_len = i;
while (text_len > 0 && text_start[text_len-1] == ' ') text_len--;
i++;
while (args[i] == ' ') i++;
j = 0;
while (args[i] && args[i] != ' ' && j < 31) fname[j++] = args[i++];
fname[j] = '\0';
if (fname[0] == '\0') {
printk("\nUsage: echo text > filename\n");
return;
}
if (fs_find(fname) == (struct minios_file*)0) fs_create(fname);
fs_write(fname, text_start, text_len);
printk("\nWrote to file\n");
}

static void meminfo(void)
{
unsigned long total_kb, used_kb, free_kb;
get_memory_info(&total_kb, &used_kb, &free_kb);
printk("\n========== Memory Info ==========\n");
printk("Total: %lu KB (%lu MB)\n", total_kb, total_kb/1024);
printk("Used:  %lu KB (%lu MB)\n", used_kb, used_kb/1024);
printk("Free:  %lu KB (%lu MB)\n", free_kb, free_kb/1024);
printk("=================================\n");
}

static void sysinfo(void)
{
printk("\n========== System Info ==========\n");
printk("OS:           %s\n", OS_NAME);
printk("Version:      %s\n", VERSION);
printk("Architecture: %s\n", OS_ARCH);
printk("Build:        %s\n", OS_BUILD);
printk("Shell:        MINI shell v1.1.12\n");
printk("=================================\n");
}

static void cmd_write(char *args)
{
char fname[32];
char text[256];
int i = 0, j = 0;

/* Get filename */
while (args[i] == ' ') i++;
j = 0;
while (args[i] && args[i] != ' ' && j < 31) {
fname[j++] = args[i++];
}
fname[j] = '\0';

if (fname[0] == '\0') {
printk("\nUsage: write <filename> <text>\n");
printk("Example: write test.txt Hello World\n");
return;
}

/* Get text (rest of the line) */
while (args[i] == ' ') i++;
j = 0;
while (args[i] && j < 255) {
text[j++] = args[i++];
}
text[j] = '\0';

if (text[0] == '\0') {
printk("\nUsage: write <filename> <text>\n");
printk("Example: write test.txt Hello World\n");
return;
}

/* Create file if it doesn't exist */
if (fs_find(fname) == (struct minios_file*)0) {
fs_create(fname);
}

/* Write to file */
fs_write(fname, text, j);
printk("\nWrote %d bytes to %s\n", j, fname);
}

static void calc(char *args)
{
int num1 = 0, num2 = 0, result = 0;
int i = 0, j = 0;
char token1[20], token2[20], op_token[10];
int op_code = 0;

/* Skip leading spaces */
while (args[i] == ' ') i++;

/* Get first number */
j = 0;
while (args[i] != ' ' && args[i] != '\0') {
token1[j++] = args[i++];
}
token1[j] = '\0';

/* Skip spaces */
while (args[i] == ' ') i++;

/* Get second number */
j = 0;
while (args[i] != ' ' && args[i] != '\0') {
token2[j++] = args[i++];
}
token2[j] = '\0';

/* Skip spaces */
while (args[i] == ' ') i++;

/* Get operator */
j = 0;
while (args[i] != ' ' && args[i] != '\0') {
op_token[j++] = args[i++];
}
op_token[j] = '\0';

/* Convert numbers */
num1 = atoi(token1);
num2 = atoi(token2);

/* Determine operator by checking first character of operator token */
if (op_token[0] != '\0') {
op_code = (int)op_token[0];
}

/* Perform calculation */
printk("\n");
if (op_code == 43 || op_code == 61) { /* + or = */
result = num1 + num2;
printk("Result: %d + %d = %d\n", num1, num2, result);
} else if (op_code == 45) { /* - */
result = num1 - num2;
printk("Result: %d - %d = %d\n", num1, num2, result);
} else if (op_code == 42 || op_code == 56) { /* * or 8 */
result = num1 * num2;
printk("Result: %d * %d = %d\n", num1, num2, result);
} else if (op_code == 47) { /* / */
if (num2 == 0) {
printk("Error: Division by zero\n");
} else {
result = num1 / num2;
printk("Result: %d / %d = %d\n", num1, num2, result);
}
} else {
printk("Error: Unknown operator (code %d)\n", op_code);
printk("Usage: calc <num1> <num2> <operator>\n");
printk("For addition use =, for multiply use 8\n");
printk("Example: calc 5 3 =\n");
}
}

static void help(void)
{
printk("\nMINI shell v1.1.12 (minios-%s)\n\n", VERSION);
printk(" calc    - calculator (calc <num1> <num2> <op>)\n");
printk(" cat     - display file contents\n");
printk(" clear   - clear screen\n");
printk(" echo    - print text or write (echo text > file)\n");
printk(" halt    - halt system\n");
printk(" help    - show this help\n");
printk(" ls      - list files\n");
printk(" mem     - memory information\n");
printk(" reboot  - reboot system\n");
printk(" rm      - remove file\n");
printk(" sysinfo - system information\n");
printk(" touch   - create file\n");
printk(" write   - write text to file (write <file> <text>)\n");
}

void proccmd(char *cmd)
{
int i, has_redirect;
if (strcmp(cmd, "clear") == 0) { clrscr(); gotoxy(0, -1); }
else if (strcmp(cmd, "halt") == 0) { printk("\nhalting...\n"); timer_wait(10); printk("[NOW]"); timer_wait(5); hlt(); }
else if (strcmp(cmd, "reboot") == 0) { printk("\nrebooting...\n"); timer_wait(10); printk("[NOW]"); timer_wait(5); reboot(); }
else if (strcmp(cmd, "help") == 0) help();
else if (strcmp(cmd, "sysinfo") == 0) sysinfo();
else if (strcmp(cmd, "mem") == 0) meminfo();
else if (strcmp(cmd, "ls") == 0) cmd_ls();
else if (strlen(cmd) > 6 && cmd[0]=='t'&&cmd[1]=='o'&&cmd[2]=='u'&&cmd[3]=='c'&&cmd[4]=='h'&&cmd[5]==' ') cmd_touch(cmd+6);
else if (strcmp(cmd, "touch") == 0) printk("\nUsage: touch <filename>\n");
else if (strlen(cmd) > 3 && cmd[0]=='r'&&cmd[1]=='m'&&cmd[2]==' ') cmd_rm(cmd+3);
else if (strcmp(cmd, "rm") == 0) printk("\nUsage: rm <filename>\n");
else if (strlen(cmd) > 4 && cmd[0]=='c'&&cmd[1]=='a'&&cmd[2]=='t'&&cmd[3]==' ') cmd_cat(cmd+4);
else if (strcmp(cmd, "cat") == 0) printk("\nUsage: cat <filename>\n");
else if (strlen(cmd) > 6 && cmd[0]=='w'&&cmd[1]=='r'&&cmd[2]=='i'&&cmd[3]=='t'&&cmd[4]=='e'&&cmd[5]==' ') cmd_write(cmd+6);
else if (strcmp(cmd, "write") == 0) printk("\nUsage: write <filename> <text>\n");
else if (strcmp(cmd, "echo") == 0) printk("\n");
else if (strlen(cmd) > 5 && cmd[0]=='c'&&cmd[1]=='a'&&cmd[2]=='l'&&cmd[3]=='c'&&cmd[4]==' ') {
calc(cmd+5);
}
else if (strlen(cmd) > 5 && cmd[0]=='e'&&cmd[1]=='c'&&cmd[2]=='h'&&cmd[3]=='o'&&cmd[4]==' ') {
has_redirect = 0;
for (i = 5; cmd[i]; i++) if (cmd[i] == '>') { has_redirect = 1; break; }
if (has_redirect) cmd_echo_to_file(cmd+5);
else printk("\n%s\n", cmd+5);
}
else printk("\nMINI shell: %s: command not found\n", cmd);
}
