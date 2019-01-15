#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include "../include/currTime.h"
#include "../include/parser.h"

static volatile int sigpipe = 0;
static void sigHandler(int signo)
{
    char ret = signo;
    write(sigpipe, &ret, 1);
}

int main(int argc, char **argv)
{
    FILE *logFile;
    int i, res, numLiveChildren;
    struct sigaction sig;
    int pipefd[2];
    struct stat st = {0};

    if (argc < 2)
    {
        printf("Usage: %s need to give atleast 1 filename as an argument!\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    logFile = fopen("logfile.log", "a");
    if (logFile == 0)
    {
        perror("Error: creating/opening the logfile failed!");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    numLiveChildren = argc - 1;
    sigpipe = pipefd[1];
    sigemptyset(&sig.sa_mask);
    sig.sa_flags= 0;
    sig.sa_handler = sigHandler;

    if (sigaction(SIGINT, &sig, 0) == -1)
    {
        perror("sigaction() with SIGINT");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGCHLD, &sig, 0) == -1)
    {
        perror("sigaction() with SIGCHLD");
        exit(EXIT_FAILURE);
    }

    if (stat("cleaned", &st) == -1)
    {
        mkdir("cleaned", 0700);
    }

    for (i = 1; i < argc; i++)
    {
        switch(fork())
        {
            case -1:
                perror("Error: fork()");
                fprintf(logFile, "ERROR [%s]: fork()", currTime("%T"));
                exit(EXIT_FAILURE);
            case 0:
                fprintf(logFile, "[%s] file %s started cleaning as child #%d. It started with PID %d\n",
                        currTime("%T"), argv[i], i, getpid());
                cleanCode(argv[i]);
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    while(numLiveChildren > 0)
    {
        char mySignal;
        res = read(pipefd[0], &mySignal, 1);
        if (res == 1)
        {
            if (mySignal == SIGINT)
            {
                printf("Received SIGINT: %d\nTerminating...\n", mySignal);
                exit(EXIT_FAILURE);
            }
            else if (mySignal == SIGCHLD)
            {
                fprintf(logFile, "[%s] Received SIGCHLD!\n", currTime("%T"));
                numLiveChildren--;
            }
        }
   }
    fclose(logFile);
    exit(EXIT_SUCCESS);
}

