#include "syslib.h"
#include <stdio.h>

int sys_unlockpriority(int proc_nr)
{
        message m;

        m.m1_i1 = proc_nr;
        printf("sys_unlockpriority kernel call: process endpoint id %d.\n", proc_nr);
        return(_taskcall(SYSTASK, SYS_UNLOCKPRIORITY, &m));
}
