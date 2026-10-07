#ifndef ENTRY_H
#define ENTRY_H

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
typedef struct {
    char *name;
    char *fullpath;
    struct stat st;
    bool is_dir;
    bool stat_valid;
} FileEntry;

typedef struct {
    FileEntry *entries;
    size_t count;
    size_t capacity;
} EntryList;

void init_entry_list(EntryList *list);
void add_entry(EntryList *list, const char *path, const char *name, bool follow_symlinks);
void free_entry_list(EntryList *list);

#endif
