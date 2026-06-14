#include <stdio.h>
#include <stdlib.h>
#include "score.h"

static char name[256] = "";

void
ask_username()
{
	printf("Name? ");
	scanf("%s", name);
}

void
store_score(int sc)
{
	FILE *out = NULL;
	char nullp[256] = "";
	int hiscore = 0;
	FILE *in = fopen("high_score.txt", "r");
	if (in != NULL) {
		fscanf(in, " %[^:]: %d", nullp, &hiscore);
		fclose(in);
	}
	
	if (sc > hiscore) {
		out = fopen("high_score.txt", "w");
		fprintf(out, "%s: %d", name, sc);
		fclose(out);

		printf("HIGH SCORE: %s: %d\n", name, sc);
	} else {
		printf("HIGH SCORE: %s: %d\n", nullp, hiscore);
	}
}
