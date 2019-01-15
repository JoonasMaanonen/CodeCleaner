#ifndef PARSER_H
#define PARSER_H

void removeComments(char c, FILE* readFp, FILE* writeFp, int* first);
void handleBlockComment(FILE* fp);
void handleSingleComment(FILE* fp);
void cleanCode(char* filename);
void removeEmptyLines();

#endif




