//   fork   //
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int ac, char **av)
{
    fork();
    fork();
    fork();
    fork();
    fork();

    printf("hello world\n");
}