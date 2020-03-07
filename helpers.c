#include <stdlib.h>
#include <string.h>

#define MAX_LINE 300
char **getparam(int *cnt, int argc, char *argv[])
{
	char **param = (char **)malloc((argc - 1) * 2 * sizeof(char *));
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && strlen(argv[i]) > 2) {
			param[*cnt] = (char *)malloc(3 * sizeof(char));
			strncpy(param[*cnt], argv[i], 2);
			(*cnt)++;
			param[*cnt] = (char *)malloc((strlen(argv[i] + 2) + 1) *
								sizeof(char));
			strcpy(param[*cnt], argv[i] + 2);
			(*cnt)++;
		}
		if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
			param[*cnt] = (char *)malloc(3 * sizeof(char));
			strcpy(param[*cnt], argv[i]);
			(*cnt)++;
		}
		if (argv[i][0] != '-') {
			param[*cnt] = (char *)malloc((strlen(argv[i]) + 1) *
								sizeof(char));
			strcpy(param[*cnt], argv[i]);
			(*cnt)++;
		}
	}
	return param;
}

void replaceWord(char s[], const char *oldW, const char *newW, int len)
{
	char result[MAX_LINE];
	int i, j, k, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i] && s[i - 1] != '\"') {
			cnt++;

			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	// Making new string of enough length

	i = 0;
	j = 0;
	k = 0;
	while (*s) {
		// compare the substring with the result
		if (strstr(s, oldW) == s && *(s - 1) != '\"') {
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
			k++;
		} else
			result[i++] = *s++;
		j++;
	}

	result[i] = '\0';
	s = s - j - k * oldWlen + k;
	memcpy(s, result, len);
}
