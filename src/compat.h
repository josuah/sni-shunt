#ifndef COMPAT_H
#define COMPAT_H

#include <stddef.h>

/** src/compat/?*.c **/
size_t strlcpy(char *buf, char const *str, size_t sz);
char * strsep(char **str_p, char const *sep);

#endif
