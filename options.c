
#include"options.h"
#include <unistd.h>
#include <stdio.h>

void init_options(Options *opts) {
    *opts = (Options){0};
    /* Default non-printable handling based on terminal output */
    if (isatty(STDOUT_FILENO)) {
        opts->opt_q = true;
    } else {
        opts->opt_w = true;
    }
}

void parse_options(int argc, char **argv, Options *opts, int *opt_index) {
    int ch;
    while ((ch = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (ch) {
            case 'A': opts->opt_A = true; break;
            case 'a': opts->opt_a = true; break;
            case 'c': opts->opt_c = true; opts->opt_u = false; break; /* -c overrides -u */
            case 'd': opts->opt_d = true; opts->opt_R = false; break; /* -d overrides -R */
            case 'F': opts->opt_F = true; break;
            case 'f': opts->opt_f = true; break;
            case 'h': opts->opt_h = true; opts->opt_k = false; break; /* -h overrides -k */
            case 'i': opts->opt_i = true; break;
            case 'k': opts->opt_k = true; opts->opt_h = false; break; /* -k overrides -h */
            case 'l': opts->opt_l = true; opts->opt_n = false; break; /* -l overrides -n */
            case 'n': opts->opt_n = true; opts->opt_l = false; break; /* -n overrides -l */
            case 'q': opts->opt_q = true; opts->opt_w = false; break; /* -q overrides -w */
            case 'R': opts->opt_R = true; opts->opt_d = false; break; /* -R overrides -d */
            case 'r': opts->opt_r = true; break;
            case 'S': opts->opt_S = true; break;
            case 's': opts->opt_s = true; break;
            case 't': opts->opt_t = true; break;
            case 'u': opts->opt_u = true; opts->opt_c = false; break; /* -u overrides -c */
            case 'w': opts->opt_w = true; opts->opt_q = false; break; /* -w overrides -q */
            default:
                break;
        }
    }
    *opt_index = optind;
}
