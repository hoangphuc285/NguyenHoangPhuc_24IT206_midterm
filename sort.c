#include "sort.h"
#include <string.h>
#include <stdlib.h>

static const Options *g_opts = NULL;

static time_t get_time(const FileEntry *e) {
    if (g_opts->opt_c) return e->st.st_ctime; /* -c option */
    if (g_opts->opt_u) return e->st.st_atime; /* -u option */
    return e->st.st_mtime;                    /* default modification time */
}

static int compare_entries(const void *a, const void *b) {
    const FileEntry *ea = (const FileEntry *)a;
    const FileEntry *eb = (const FileEntry *)b;
    int res = 0;

    if (g_opts->opt_S) { /* Sort by size */
        if (eb->st.st_size > ea->st.st_size) res = 1;
        else if (eb->st.st_size < ea->st.st_size) res = -1;
    } else if (g_opts->opt_t) { /* Sort by time */
        time_t ta = get_time(ea);
        time_t tb = get_time(eb);
        if (tb > ta) res = 1;
        else if (tb < ta) res = -1;
    }

    if (res == 0) {
        res = strcmp(ea->name, eb->name);
    }

    return g_opts->opt_r ? -res : res;
}

void sort_entries(EntryList *list, const Options *opts) {
    if (opts->opt_f) return; /* -f disables sorting */
    g_opts = opts;
    qsort(list->entries, list->count, sizeof(FileEntry), compare_entries);
}
