#ifndef READ_TCB_H
#define READ_TCB_H

#include <error.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define PATH_TCB    "/etc/tcb/"

static void print_shadow(struct spwd *sp);
void shadow_keys(char *path);
void print_shadow_tcb(char *path);

#endif // READ_TCB_H
