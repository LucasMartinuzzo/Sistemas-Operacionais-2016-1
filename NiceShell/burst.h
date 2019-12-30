#ifndef BURST_H
#define BURST_H

#include "tad.h"

void set_min_priority(void);
int set_max_priority(pid_t);
void burst(cPIDList *);

#endif
