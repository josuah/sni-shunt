#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "envfmt.h"

int
envfmt_parse(char *s, char **env, char **fmt)
{
	*fmt = s;
	*env = strsep(fmt, "=");
	if (*env == NULL)
		return -1;
	return 0;
}

#include "log.h"

int
envfmt_export(struct envfmt *ef, char *sni)
{
	struct mem_pool pool = {0};
	struct str str;
	int e = -1;

	if (str_init(&str, &pool) < 0)
		goto end;
	for (char *fmt = ef->fmt; *fmt != '\0';) {
		if (strncmp(fmt, "%%", 2) == 0) {
			if (str_append_char(&str, '%') < 0)
				goto end;
			fmt += 2;
		} else if (strncmp(fmt, "%s", 2) == 0) {
			if (str_append_string(&str, sni) < 0)
				goto end;
			fmt += 2;
		} else {
			if (str_append_char(&str, *fmt) < 0)
				goto end;
			fmt += 1;
		}
	}
	e = setenv(ef->env, str_c(&str), 1);
end:
	mem_free(&pool);
	return e;
}

struct envfmt *
envfmt_new(char *env, char *fmt,
	struct mem_pool *pool)
{
	struct envfmt *new;

	new = mem_alloc(pool, sizeof *new);
	if (new == NULL)
		return NULL;
	new->env = env;
	new->fmt = fmt;
	return new;
}

int
envfmt_add_new(struct envfmt **list, char *env, char *fmt,
	struct mem_pool *pool)
{
	struct envfmt *new;

	new = envfmt_new(env, fmt, pool);
	if (new == NULL)
		return -1;
	new->next = *list;
	*list = new;
	return 0;
}
