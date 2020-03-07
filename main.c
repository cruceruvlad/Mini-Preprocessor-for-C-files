#include<stdio.h>
#include<string.h>
#include "helpers.h"
#include "hashmap.h"

int main(int argc, char *argv[])
{
	char **param, *tokenkey, *tokenval, buf[MAX_LINE], bufcpy[MAX_LINE];
	char *curDir, **inclDir;
	int cnt = 0, rc;
	int in_main = 0, i, trueif = 1, inclcnt = 0;
	FILE *in = stdin, *out = stdout, *incl = NULL;
	struct table *t = createTable(TABLE_SIZE);

	inclDir = (char **)malloc(2*argc*sizeof(char *));
	param = getparam(&cnt, argc, argv);
	for (i = 0; i < cnt; i++) {
		if (!strcmp(param[i], "-D")) {
			DIE(i == cnt-1, "bad arg");
			i++;
			DIE(param[i][0] == '-', "bad arg");
			tokenkey = strtok(param[i], "=");
			tokenval = strtok(NULL, "");
			if (tokenval != NULL)
				insert(t, tokenkey, tokenval);
			else
				insert(t, tokenkey, "");
			continue;
		}
		if (!strcmp(param[i], "-I")) {
			DIE(i == cnt-1, "bad arg");
			i++;
			DIE(param[i][0] == '-', "bad arg");
			inclDir[inclcnt] =
			(char *)malloc(MAX_LINE*sizeof(char));
			strcpy(inclDir[inclcnt], "./");
			strcat(inclDir[inclcnt], param[i]);
			strcat(inclDir[inclcnt], "/");
			inclcnt++;
			continue;
		}
		if (!strcmp(param[i], "-o")) {
			DIE(i == cnt-1 || out != stdout, "bad arg");
			i++;
			DIE(param[i][0] == '-', "bad arg");
			out = fopen(param[i], "w");
			DIE(out == NULL, "open out file");
			continue;
		}
		DIE(in != stdin && out != stdout, "bad arg");
		if (in == stdin) {
			in = fopen(param[i], "r");
			DIE(in == NULL, "open in file");
			curDir = (char *)malloc(strlen(param[i])*sizeof(char));
			*(strrchr(param[i], (int)'/')+1) = '\0';
			strcpy(curDir, param[i]);
		} else {
			out = fopen(param[i], "w");
			DIE(out == NULL, "open out file");
		}
	}
	while (1) {
		if (incl == NULL) {
			fgets(buf, MAX_LINE, in);
			DIE(ferror(in), "reading error");
			if (feof(in))
				break;
		} else {
			fgets(buf, MAX_LINE, incl);
			DIE(ferror(incl), "reading error");
			if (feof(incl)) {
				rc = fclose(incl);
				DIE(rc < 0, "closing incl file err");
				incl = NULL;
				continue;
			}
		}
		if (trueif == 0 && strncmp(buf, "#elif", strlen("#elif")) &&
		strncmp(buf, "#else", strlen("#else")) &&
		strncmp(buf, "#endif", strlen("#endif")))
			continue;
		if (!strncmp(buf, "#ifndef", strlen("#ifndef"))) {
			tokenkey = strtok(buf+strlen("#ifndef")+1, "\n");
			tokenval = lookup(t, tokenkey);
			if (tokenval == NULL)
				trueif = 1;
			else
				trueif = 0;
			continue;
		}

		if (!strncmp(buf, "#include \"", strlen("#include\""))) {
			tokenkey = strtok(buf+strlen("#include\"")+1, "\"");
			for (i = 0; i < inclcnt; i++) {
				incl = fopen(strcat(inclDir[i], tokenkey), "r");
				*(strrchr(inclDir[i], (int)'/')+1) = '\0';
				if (incl != NULL)
					break;
			}
			if (incl != NULL)
				continue;
			if (in != stdin) {
				incl = fopen(strcat(curDir, tokenkey), "r");
				*(strrchr(curDir, (int)'/')+1) = '\0';
			} else
				incl = fopen(tokenkey, "r");
			DIE(incl == NULL, "open incl file");
			continue;
		}
		if (!strncmp(buf, "#ifdef", strlen("#ifdef"))) {
			tokenkey = strtok(buf+strlen("#ifdef")+1, "\n");
			tokenval = lookup(t, tokenkey);
			if (tokenval != NULL)
				trueif = 1;
			else
				trueif = 0;
			continue;
		}
		if (!strncmp(buf, "#undef", strlen("#undef"))) {
			tokenkey = strtok(buf+strlen("#undef")+1, "\n");
			delete(t, tokenkey);
			continue;
		}
		if (!strncmp(buf, "#else", strlen("#else"))) {
			trueif = !trueif;
			continue;
		}
		if (!strncmp(buf, "#endif", strlen("#endif"))) {
			trueif = 1;
			continue;
		}
		memcpy(bufcpy, buf, MAX_LINE);
		tokenkey = strtok(bufcpy, delim);
		while (tokenkey) {
			tokenval = lookup(t, tokenkey);
			if (tokenval)
				replaceWord(buf, tokenkey, tokenval, MAX_LINE);
			tokenkey = strtok(NULL, delim);
		}
		if (!strncmp(buf, "#if", strlen("#if"))) {
			tokenkey = strtok(buf+strlen("#if")+1, "\n");
			if (atoi(tokenkey) > 0)
				trueif = 1;
			else
				trueif = 0;
			continue;
		}
		if (!strncmp(buf, "#elif", strlen("#elif"))) {
			tokenkey = strtok(buf+strlen("#elif")+1, "\n");
			if (atoi(tokenkey) > 0)
				trueif = 1;
			else
				trueif = 0;
			continue;
		}
		if (!strncmp(buf, "#define", strlen("#define"))) {
			tokenkey = strtok(buf+strlen("#define"), "\n ");
			tokenval = strtok(NULL, "\n");
			if (tokenval != NULL)
				insert(t, tokenkey, tokenval);
			else
				insert(t, tokenkey, "");
			continue;
		}
		if (!(!strcmp(buf, "\n") && in_main == 0)) {
			if (strstr(buf, "int mai") != NULL)
				in_main = 1;
			rc = fputs(buf, out);
			DIE(rc < 0, "writing error");
		}
	}
	if (in != stdin) {
		rc = fclose(in);
		DIE(rc < 0, "closing in file err");
		free(curDir);
	}
	if (out != stdout) {
		rc = fclose(out);
		DIE(rc < 0, "closing out file err");
	}
	for (i = 0; i < inclcnt; i++)
		free(inclDir[i]);
	free(inclDir);
	for (i = 0; i < cnt; i++)
		free(param[i]);
	free(param);
	deleteTable(t);
	return 0;
}
