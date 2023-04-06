
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
 
int main(void) {
  char *binaryPath = "/bin/ls";
  //char *binaryPath = "ls";
  char *args[] = {binaryPath, "-l", "/home", NULL};
  execv(binaryPath, args);
  perror("Return from execvp() not expected");
  return 0;
}
