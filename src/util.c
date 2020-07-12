#include "util.h"

#include <string.h>

size_t
strlcpy(char *buf, const char *str, size_t sz)
{
	size_t len, cpy;

	len = strlen(str);
	cpy = (len > sz) ? sz : len;
	memcpy(buf, str, cpy);
	buf[cpy] = '\0';
	return len;
}

char *
strsep(char **str_p, char const *sep)
{
	char *s, *prev;

	if (*str_p == NULL)
		return NULL;

	for (s = prev = *str_p; strchr(sep, *s) == NULL; s++)
		continue;

	if (*s == '\0') {
		*str_p = NULL;
	} else {
		*s = '\0';
		*str_p = s + 1;
	}
	return prev;
}
