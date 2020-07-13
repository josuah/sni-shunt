#include "mem.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

static struct mem_block *
mem_block(void *v)
{
	struct mem_block *block = (void *)((char *)v - sizeof *block);

	assert(memcmp(block->magic, MEM_BLOCK_MAGIC, 8) == 0);
	return block;
}

void *
mem_alloc(struct mem_pool *pool, size_t len)
{
	struct mem_block *block;

	block = calloc(1, sizeof *block + len);
	if (block == NULL)
		return NULL;
	memcpy(block->magic, MEM_BLOCK_MAGIC, 8);

	block->len = len;
	block->pool = pool;
	block->prev = NULL;
	block->next = pool->head;
	if (pool->head != NULL)
		pool->head->prev = block;
	pool->head = block;

	return block->buf;
}

int
mem_resize(void **pp, size_t len)
{
	struct mem_block *block = mem_block(*pp);
	int first = (block == block->pool->head);
	void *v;

	v = realloc(block, sizeof *block + len);
	if (v == NULL)
		return -1;
	block = v;

	if (len > block->len)
		memset(block->buf + block->len, 0, len - block->len);

	block->len = len;
	if (block->prev != NULL)
		block->prev->next = v;
	if (block->next != NULL)
		block->next->prev = v;
	if (first)
		block->pool->head = v;
	*pp = block->buf;

	assert(memcmp(block->magic, MEM_BLOCK_MAGIC, 8) == 0);
	return 0;
}

int
mem_grow(void **pp, size_t len)
{
	return mem_resize(pp, mem_length(*pp) + len);
}

void
mem_shrink(void *v, size_t len)
{
	struct mem_block *block = mem_block(v);

	if (len <= block->len)
		block->len -= len;
}

size_t
mem_length(void *v)
{
	struct mem_block *block = mem_block(v);
	return block->len;
}

int
mem_append(void **pp, char const *buf, size_t len)
{
	size_t old_len = mem_length(*pp);
	struct mem_block *block;

	if (mem_grow(pp, len) < 0)
		return -1;
	block = mem_block(*pp);
	memcpy((char *)block->buf + old_len, buf, len);

	assert(memcmp(block->magic, MEM_BLOCK_MAGIC, 8) == 0);
	return 0;
}

void
mem_delete(void *v)
{
	struct mem_block *block = mem_block(v);;

	if (block == block->pool->head)
		block->pool->head = block->next;
	if (block->next != NULL)
		block->next->prev = block->prev;
	if (block->prev != NULL)
		block->prev->next = block->next;
	memset(block, 0, sizeof *block);
	free(block);
}

void
mem_free(struct mem_pool *pool)
{
	struct mem_block *block, *next;

	for (block = pool->head; block != NULL; block = next) {
		next = block->next;
		memset(block, 0, sizeof *block);
		free(block);
	}
}
