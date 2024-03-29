#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define MAXSIZE 128
#define KEY     5423    

#define PIECES 6


struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

const char * getError();

#include "constante.c"