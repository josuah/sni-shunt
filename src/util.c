#include <string.h>

#include "util.h"

char *
strsep(char **strp, const char *sep)
{
	char *s, *prev;

	if (*strp == NULL)
		return NULL;
	for (s = prev = *strp; strchr(sep, *s) == NULL; s++);
	if (*s == '\0') {
		*strp = NULL;
		return prev;
	}
	*s = '\0';
	*strp = s + 1;

	return prev;
}
