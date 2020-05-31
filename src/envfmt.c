#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "envfmt.h"

int
envfmt_parse(char *s, char **env, char **fmt)
{
	char *cp;

	*fmt = s;
	if ((*env = strsep(fmt, "=")) == NULL)
		return -1;
	if ((cp = strchr(*fmt, '%')) == NULL || cp[1] != 's')
		return -1;
	if ((cp = strchr(cp + 1, '%')) != NULL)
		return -1;
	return 0;
}

int
envfmt_export(struct envfmt *ef, char const *sni)
{
	char buf[1024];

	if (snprintf(buf, sizeof(buf), ef->fmt, sni) >= (int)sizeof(buf))
		return errno=ENAMETOOLONG, -1;
	if (setenv(ef->env, buf, 1))
		return -1;
	return 0;
}

void
envfmt_free(struct envfmt *ef)
{
	if (ef->next != NULL)
		envfmt_free(ef->next);
	free(ef);
}

struct envfmt *
envfmt_new(char *env, char *fmt)
{
	struct envfmt *new;

	if ((new = calloc(sizeof(*new), 1)) == NULL)
		return NULL;
	new->env = env;
	new->fmt = fmt;
	return new;
}

int
envfmt_add_new(struct envfmt **list, char *env, char *fmt)
{
	struct envfmt *new;

	if ((new = envfmt_new(env, fmt)) == NULL)
		return -1;
	new->next = *list;
	*list = new;
	return 0;
}
