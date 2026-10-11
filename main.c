#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "options.h"
#include "entry.h"
#include "sort.h"
#include "display.h"

void process_directory(const char *dir_path, const Options *opts, bool print_header) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror(dir_path);
        return;
    }

    if (print_header) {
        printf("\n%s:\n", dir_path);
    }

    EntryList list;
    init_entry_list(&list);

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        /* Filter . and .. entries based on -a and -A options */
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            if (!opts->opt_a) continue;
        } else if (dp->d_name[0] == '.' && !opts->opt_a && !opts->opt_A) {
            continue;
        }

        add_entry(&list, dir_path, dp->d_name, false);
    }
    closedir(dir);

    sort_entries(&list, opts);
    print_entries(&list, opts);

    /* -R option handling */
    if (opts->opt_R) {
        for (size_t i = 0; i < list.count; i++) {
            FileEntry *e = &list.entries[i];
            if (e->is_dir && strcmp(e->name, ".") != 0 && strcmp(e->name, "..") != 0) {
                process_directory(e->fullpath, opts, true);
            }
        }
    }

    free_entry_list(&list);
}

int main(int argc, char **argv) {
    Options opts;
    init_options(&opts);

    int opt_index = 0;
    parse_options(argc, argv, &opts, &opt_index);

    EntryList files, dirs;
    init_entry_list(&files);
    init_entry_list(&dirs);

    int num_operands = argc - opt_index;

    if (num_operands == 0) {
        process_directory(".", &opts, false);
    } else {
        /* Separate files and directories as per specification */
        for (int i = opt_index; i < argc; i++) {
            FileEntry e;
            memset(&e, 0, sizeof(e));
            e.name = strdup(argv[i]);
            e.fullpath = strdup(argv[i]);

            if (lstat(argv[i], &e.st) == 0) {
                e.stat_valid = true;
                e.is_dir = S_ISDIR(e.st.st_mode);

                if (e.is_dir && !opts.opt_d) {
                    add_entry(&dirs, "", argv[i], true);
                } else {
                    add_entry(&files, "", argv[i], false);
                }
            } else {
                perror(argv[i]);
            }
            free(e.name);
            free(e.fullpath);
        }

        /* Print non-directory files first */
        if (files.count > 0) {
            sort_entries(&files, &opts);
            print_entries(&files, &opts);
        }

        /* Process directory operands */
        if (dirs.count > 0) {
            sort_entries(&dirs, &opts);
            for (size_t i = 0; i < dirs.count; i++) {
                bool print_hdr = (num_operands > 1 || files.count > 0);
                process_directory(dirs.entries[i].name, &opts, print_hdr);
            }
        }
    }

    free_entry_list(&files);
    free_entry_list(&dirs);
    return 0;
}
