#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

void format_mode(mode_t mode, char *str) {
    /* Type classification */
    if (S_ISDIR(mode))  str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else if (S_ISSOCK(mode)) str[0] = 's';
    else str[0] = '-';

    /* Owner permissions */
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) str[3] = (mode & S_IXUSR) ? 's' : 'S';
    else str[3] = (mode & S_IXUSR) ? 'x' : '-';

    /* Group permissions */
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) str[6] = (mode & S_IXGRP) ? 's' : 'S';
    else str[6] = (mode & S_IXGRP) ? 'x' : '-';

    /* Other permissions */
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) str[9] = (mode & S_IXOTH) ? 't' : 'T';
    else str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';
}

static void print_filename(const char *name, const Options *opts) {
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isprint((unsigned char)name[i]) && opts->opt_q) {
            putchar('?');
        } else {
            putchar(name[i]);
        }
    }
}

static char get_classifier(mode_t mode) {
    if (S_ISDIR(mode)) return '/';
    if (S_ISLNK(mode)) return '@';
    if (S_ISSOCK(mode)) return '=';
    if (S_ISFIFO(mode)) return '|';
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) return '*';
    return '\0';
}

void print_entries(const EntryList *list, const Options *opts) {
    /* Calculate block count total for long listing if required */
    if ((opts->opt_l || opts->opt_n || opts->opt_s) && list->count > 0) {
        long total_blocks = 0;
        for (size_t i = 0; i < list->count; i++) {
            if (list->entries[i].stat_valid) {
                total_blocks += list->entries[i].st.st_blocks;
            }
        }
        /* Display block size total (in 512-byte blocks standard or scaled) */
        printf("total %ld\n", total_blocks / 2);
    }

    for (size_t i = 0; i < list->count; i++) {
        const FileEntry *e = &list->entries[i];

        if (!e->stat_valid) continue;

        /* -i option: inode numbers */
        if (opts->opt_i) {
            printf("%llu ", (unsigned long long)e->st.st_ino);
        }

        /* -s option: block count */
        if (opts->opt_s) {
            printf("%lld ", (long long)(e->st.st_blocks / 2));
        }

        /* -l or -n long options */
        if (opts->opt_l || opts->opt_n) {
            char mode_str[11];
            format_mode(e->st.st_mode, mode_str);
            printf("%s %2u ", mode_str, (unsigned int)e->st.st_nlink);

            /* Owner & Group handling */
            struct passwd *pwd = getpwuid(e->st.st_uid);
            struct group *grp = getgrgid(e->st.st_gid);

            if (opts->opt_n || !pwd) {
                printf("%u ", e->st.st_uid);
            } else {
                printf("%s ", pwd->pw_name);
            }

            if (opts->opt_n || !grp) {
                printf("%u ", e->st.st_gid);
            } else {
                printf("%s ", grp->gr_name);
            }

            printf("%8lld ", (long long)e->st.st_size);

            /* Date formatting */
            time_t t = opts->opt_c ? e->st.st_ctime : (opts->opt_u ? e->st.st_atime : e->st.st_mtime);
            char date_buf[64];
            struct tm *tm_info = localtime(&t);
            strftime(date_buf, sizeof(date_buf), "%b %e %H:%M", tm_info);
            printf("%s ", date_buf);
        }

        print_filename(e->name, opts);

        /* -F classifier */
        if (opts->opt_F) {
            char c = get_classifier(e->st.st_mode);
            if (c != '\0') putchar(c);
        }

        /* Symbolic link destination printing under long format */
        if ((opts->opt_l || opts->opt_n) && S_ISLNK(e->st.st_mode)) {
            char link_target[1024];
            ssize_t len = readlink(e->fullpath, link_target, sizeof(link_target) - 1);
            if (len != -1) {
                link_target[len] = '\0';
                printf(" -> %s", link_target);
            }
        }

        putchar('\n');
    }
}
