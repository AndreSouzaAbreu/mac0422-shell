#include <lib.h>
#include <unistd.h>
#include <stdio.h>

PUBLIC int lockpriority(pid_t p, int priority) {
  message m;

  m.m1_i1 = p;
  m.m1_i2 = priority;
  printf("POSIX lib lockpriority: pid %d priority %d\n", p, priority);
  return(_syscall(MM, LOCK_PRIORITY, &m));
}
