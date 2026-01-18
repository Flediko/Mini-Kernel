/*
 * fs/vfs.c - Simple in-memory file system for MiniOS
 */

#include <string.h>
#include <minios/fs.h>
#include <minios/kernel.h>

/* File table (stored in memory) - exported for shell access */
struct minios_file file_table[MAX_FILES];
static char file_buffers[MAX_FILES][MAX_FILE_SIZE];

/* Initialize file system */
void fs_init(void)
{
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		file_table[i].used = 0;
		file_table[i].size = 0;
		file_table[i].name[0] = '\0';
		file_table[i].content = file_buffers[i];
	}
}

/* Find a file by name */
struct minios_file* fs_find(const char *filename)
{
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		if (file_table[i].used && strcmp(file_table[i].name, filename) == 0) {
			return &file_table[i];
		}
	}
	return (struct minios_file*)0;  /* NULL */
}

/* Create a new file */
int fs_create(const char *filename)
{
	int i;
	
	/* Check if file already exists */
	if (fs_find(filename) != (struct minios_file*)0) {
		return -1;  /* File already exists */
	}
	
	/* Find a free slot */
	for (i = 0; i < MAX_FILES; i++) {
		if (!file_table[i].used) {
			file_table[i].used = 1;
			file_table[i].size = 0;
			
			/* Copy filename */
			int j = 0;
			while (filename[j] && j < MAX_FILENAME - 1) {
				file_table[i].name[j] = filename[j];
				j++;
			}
			file_table[i].name[j] = '\0';
			
			return 0;  /* Success */
		}
	}
	
	return -2;  /* No free slots */
}

/* Delete a file */
int fs_delete(const char *filename)
{
	struct minios_file *file = fs_find(filename);
	
	if (file == (struct minios_file*)0) {
		return -1;  /* File not found */
	}
	
	file->used = 0;
	file->size = 0;
	file->name[0] = '\0';
	
	return 0;  /* Success */
}

/* Write data to a file */
int fs_write(const char *filename, const char *data, int size)
{
	struct minios_file *file = fs_find(filename);
	int i;
	
	if (file == (struct minios_file*)0) {
		return -1;  /* File not found */
	}
	
	if (size > MAX_FILE_SIZE) {
		size = MAX_FILE_SIZE;
	}
	
	/* Copy data */
	for (i = 0; i < size; i++) {
		file->content[i] = data[i];
	}
	
	file->size = size;
	
	return size;  /* Return bytes written */
}

/* Read data from a file */
int fs_read(const char *filename, char *buffer, int size)
{
	struct minios_file *file = fs_find(filename);
	int i;
	int bytes_to_read;
	
	if (file == (struct minios_file*)0) {
		return -1;  /* File not found */
	}
	
	bytes_to_read = file->size < size ? file->size : size;
	
	/* Copy data */
	for (i = 0; i < bytes_to_read; i++) {
		buffer[i] = file->content[i];
	}
	
	return bytes_to_read;  /* Return bytes read */
}

/* List all files */
int fs_list(void)
{
	int i;
	int count = 0;
	
	for (i = 0; i < MAX_FILES; i++) {
		if (file_table[i].used) {
			count++;
		}
	}
	
	return count;
}