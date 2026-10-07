#ifndef DISPLAY_H
#define DISPLAY_H

#include "entry.h"
#include "options.h"

void print_entries(const EntryList *list, const Options *opts);
void format_mode(mode_t mode, char *str);

#endif
