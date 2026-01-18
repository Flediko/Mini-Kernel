#ifndef _FS_H
#define _FS_H

#define MAX_FILENAME 32
#define MAX_FILES 16
#define MAX_FILE_SIZE 4096

/* Simple file structure */
struct minios_file {
	char name[MAX_FILENAME];
	char *content;
	int size;
	int used;  /* 1 if slot is used, 0 if free */
};

/* File system operations */
extern void fs_init(void);
extern int fs_create(const char *filename);
extern int fs_delete(const char *filename);
extern int fs_write(const char *filename, const char *data, int size);
extern int fs_read(const char *filename, char *buffer, int size);
extern int fs_list(void);
extern struct minios_file* fs_find(const char *filename);

#endif