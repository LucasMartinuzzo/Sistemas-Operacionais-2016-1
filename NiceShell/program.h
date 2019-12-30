#ifndef PROGRAM_H
#define PROGRAM_H

#include "tad.h"

/*Tipo enumerado utilizado somente para distinguir um processo em foreground de um em background*/
typedef enum {FG, BG} type_stat;

type_stat verify_background(char ***, int *);
void execute(char **, int, cPIDList *);

#endif
