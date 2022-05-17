#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  sleep(2);
  printf("i'm here\n");
  sleep(3);
  printf("i'm still there\n");
  return 0;
}
