#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
  printf("FLAG");
  execlp("open", "open", "-a", "Safari", "http://apple.com/", NULL);
  perror("Return from execlp() not expected");
  exit(EXIT_FAILURE);
}