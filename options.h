#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef struct {
    bool opt_A; /* -A: list all except . and .. */
    bool opt_a; /* -a: list all including . and .. */
    bool opt_c; /* -c: status change time */
    bool opt_d; /* -d: list directories as plain files */
    bool opt_F; /* -F: classify entries */
    bool opt_f; /* -f: unsorted */
    bool opt_h; /* -h: human-readable sizes */
    bool opt_i; /* -i: inode numbers */
    bool opt_k; /* -k: kilobyte block sizes */
    bool opt_l; /* -l: long listing format */
    bool opt_n; /* -n: numeric owner and group IDs */
    bool opt_q; /* -q: force '?' for non-printable characters */
    bool opt_R; /* -R: recursive listing */
    bool opt_r; /* -r: reverse sort order */
    bool opt_S; /* -S: sort by size */
    bool opt_s; /* -s: block allocation size */
    bool opt_t; /* -t: sort by time */
    bool opt_u; /* -u: access time */
    bool opt_w; /* -w: raw non-printable characters */
} Options;

void init_options(Options *opts);
void parse_options(int argc, char **argv, Options *opts, int *opt_index);

#endif
