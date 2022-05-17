#include "syslib.h"
#include <stdio.h>

int sys_lockpriority(int proc_nr, int priority)
{
        message m;

        m.m1_i1 = proc_nr;
        m.m1_i2 = priority;
        printf("sys_lockpriority kernel call: process endpoint id %d and priority %d.\n",
                        proc_nr, priority);
        return(_taskcall(SYSTASK, SYS_LOCKPRIORITY, &m));
}
