#include <string.h>

#include "util.h"

size_t
strlcpy(char *buf, const char *str, size_t sz)
{
	size_t len, cpy;

	cpy = ((len = strlen(str)) > sz) ? (sz) : (len);
	memcpy(buf, str, cpy);
	buf[sz - 1] = '\0';
	return len;
}

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
