#ifndef ENVFMT_H
#define ENVFMT_H

#include "mem.h"
#include "str.h"

struct envfmt {
	char *env;
	char *fmt;
	struct envfmt *next;
};

/** src/envfmt.c **/
int envfmt_parse(char *s, char **env, char **fmt);
int envfmt_export(struct envfmt *ef, char *sni);
struct envfmt * envfmt_new(char *env, char *fmt, struct mem_pool *pool);
int envfmt_add_new(struct envfmt **list, char *env, char *fmt, struct mem_pool *pool);

#endif
