#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t c = fork();
  if (c == 0) {
    sleep(1);
    return 0;
  }

  int pidfd = syscall(SYS_pidfd_open, c, 0);
  if (pidfd < 0) {
    perror("pidfd_open");
    return 2;
  }

  int fd = syscall(SYS_pidfd_getfd, pidfd, 1, 0);
  if (fd < 0) {
    perror("pidfd_getfd");
    printf("System is not vulnerable (ptrace/pidfd_getfd() is not usable)\n");
    return 0;
  }

  printf("System might be vulnerable (ptrace/pidfd_getfd() is usable)\n");
  return 1;
}
