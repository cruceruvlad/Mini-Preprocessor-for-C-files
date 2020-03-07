#ifndef HELPERS_H
#define HELPERS_H

#include<stdlib.h>

#define TABLE_SIZE 20
#define MAX_LINE 300
#define DIE(assertion, call_description)\
	do { \
		if (assertion) { \
			fprintf(stderr, "(%s, %d): ", \
					__FILE__, __LINE__); \
			perror(call_description); \
			exit(EXIT_FAILURE); \
		} \
	} while (0)

char delim[] = "\t[]{}<>=+-* /%!&|^\n.,:;()\\";

char **getparam(int *cnt, int argc, char *argv[]);

void replaceWord(char s[], const char *oldW, const char *newW, int len);

#endif
