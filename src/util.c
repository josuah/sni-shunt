#include <string.h>

#include "util.h"

size_t
strlcpy(char *buf, const char *str, size_t sz)
{
	size_t len, cpy;

	cpy = ((len = strlen(str)) > sz) ? (sz) : (len);
	memcpy(buf, str, cpy + 1);
	buf[sz - 1] = '\0';
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
