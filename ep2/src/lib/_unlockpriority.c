#include <lib.h>
#include <unistd.h>
#include <stdio.h>

PUBLIC int unlockpriority(pid_t p) {
  message m;

  m.m1_i1 = p;
  printf("POSIX lib unlockpriority: pid %d\n", p);
  return(_syscall(MM, UNLOCK_PRIORITY, &m));
}
