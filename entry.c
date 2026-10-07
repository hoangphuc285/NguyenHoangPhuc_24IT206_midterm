#include "entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_entry_list(EntryList *list) {
    list->entries = NULL;
    list->count = 0;
    list->capacity = 0;
}

void add_entry(EntryList *list, const char *path, const char *name, bool follow_symlinks) {
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 16 : list->capacity * 2;
        list->entries = realloc(list->entries, list->capacity * sizeof(FileEntry));
    }

    FileEntry *e = &list->entries[list->count];
    e->name = strdup(name);

    if (path && strlen(path) > 0) {
        size_t len = strlen(path) + strlen(name) + 2;
        e->fullpath = malloc(len);
        snprintf(e->fullpath, len, "%s/%s", path, name);
    } else {
        e->fullpath = strdup(name);
    }

    int res;
    if (follow_symlinks) {
        res = stat(e->fullpath, &e->st);
    } else {
        res = lstat(e->fullpath, &e->st);
    }

    if (res == 0) {
        e->stat_valid = true;
        e->is_dir = S_ISDIR(e->st.st_mode);
    } else {
        e->stat_valid = false;
        e->is_dir = false;
        perror(e->fullpath);
    }

    list->count++;
}

void free_entry_list(EntryList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->entries[i].name);
        free(list->entries[i].fullpath);
    }
    free(list->entries);
    init_entry_list(list);
}
