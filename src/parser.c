#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include "parser.h"


// This parses single comments
void handleSingleComment(FILE* fp)
{
    char c;
    while((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            return;
        }
    }
}

// This parses the block comments
void handleBlockComment(FILE* fp)
{
    char c, d;
    while((c = fgetc(fp)) != EOF)
    {
        if (d == '*')
        {
            d = fgetc(fp);
            if (d == '/')
            {
                return;
            }
        }
    }
}

// This removes all comments and emptylines.
void removeComments(char c, FILE* readFp, FILE* writeFp, int* first)
{
    char nextC;
    // Both comment styles start with one bracket
    if (c == '/')
    {
        if ((nextC = fgetc(readFp)) == '*')
        {
            handleBlockComment(readFp);
        }
        else if (nextC == '/')
        {
            handleSingleComment(readFp);
            return;
        }
        else
        {
            fputc(c, writeFp);
            fputc(nextC, writeFp);
        }
    }
    else if (c == '\n')
    {
        if (*first == 0)
        {
            *first = 0;
            return;
        }
        else
        {
            *first = 0;
            fputc(c, writeFp);
            return;
        }
    }
    else
    {
        fputc(c, writeFp);
    }
    *first = 1;
}


void cleanCode(char *filename)
{
    FILE *readFp, *writeFp;
    char c;
    int first = 0;
    int *firstP = &first;
    char writeFileName[strlen(filename) + strlen(".clean") + strlen("cleaned/")];

    readFp = fopen(filename, "r");

    if (readFp == 0)
    {
        perror("fopen() readFp");
        exit(EXIT_FAILURE);
    }

    // Lock the uncleaned file.
    int result = flock(fileno(readFp), LOCK_EX);
    if (result == -1)
    {
        perror("flock() on readFP");
        exit(EXIT_FAILURE);
    }


    // Specify the String used to create a new .cleaned file for writing.
    sprintf(writeFileName, "cleaned/%s.clean", filename);

    writeFp = fopen(writeFileName, "w");
    if (writeFp == 0)
    {
        perror("fopen() writeFp");
        fprintf(stderr, "Error: opening file %s\n", writeFileName);
        exit(EXIT_FAILURE);
    }

    // Lock the .clean file
    result = flock(fileno(writeFp), LOCK_EX);
    if (result == -1)
    {
        perror("flock() on readFP");
        exit(EXIT_FAILURE);
    }

    // Now lets read the file data 1 char at a time
    while((c = fgetc(readFp)) != EOF)
    {
        removeComments(c, readFp, writeFp, firstP);
    }
    fclose(readFp);
    fclose(writeFp);
}


