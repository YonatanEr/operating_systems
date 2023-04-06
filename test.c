#include <stdio.h>
#include <string.h>


int main ()
{
  //char str[] ="- This,       a sample string.";
  //char str[] ="";
  char str[] ="        a       v      ";
  char * pch;
  printf("Splitting string \"%s\" into tokens:\n\n\n",str);
  pch = strtok (str," ");
  while (pch != NULL)
  {
    printf("token = %s\n",pch);
    int d = strlen(pch);
    printf("len = %d\n\n\n\n\n",d);
    pch = strtok (NULL, " ,.");
  }
  return 0;
}