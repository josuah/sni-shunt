#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"
#include "envfmt.h"

struct {
	char *arg0, **argv;
	char *flag['z'];
} conf = {0};

void
default_name(void)
{
	warn("server name not found or incorrect input");
	execvp(conf.argv[0], conf.argv);
	die("execvp(2) into arguments");
}

void
narrow_len(size_t *input_len, size_t len)
{
	if (*input_len < len)
		default_name();
	*input_len = len;
}

void
skip(char **input_buf, size_t *input_len, size_t len)
{
	*input_buf += len;
	*input_len -= len;
}

void
get_buf(char **input_buf, size_t *input_len, void *buf, size_t len)
{
	if (*input_len < len)
		default_name();
	memcpy(buf, *input_buf, len);
	skip(input_buf, input_len, len);
}

void
expect(char **input_buf, size_t *input_len, char *buf, size_t len)
{
	if (*input_len < len)
		default_name();
	if (memcmp(*input_buf, buf, len) != 0)
		default_name();
	skip(input_buf, input_len, len);
}

void
get_u32(char **input_buf, size_t *input_len, uint32_t *u32)
{
	get_buf(input_buf, input_len, u32, sizeof(uint32_t));
	*u32 = ntohs(*u32);
}

void
get_u24(char **input_buf, size_t *input_len, uint32_t *u32)
{
	*u32 = 0;
	get_buf(input_buf, input_len, u32 + 1, 3);
	*u32 = ntohs(*u32);
}

void
get_u16(char **input_buf, size_t *input_len, uint16_t *u16)
{
	get_buf(input_buf, input_len, u16, sizeof(uint16_t));
	*u16 = ntohs(*u16);
}

void
get_u8(char **input_buf, size_t *input_len, uint8_t *u8)
{
	get_buf(input_buf, input_len, u8, sizeof(uint8_t));
}

void
parse(char *buf, char *server_name, size_t len)
{
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

	debug("record header");
	expect(&buf, &len, "\x16", 1);
	skip(&buf, &len, 2);
	get_u16(&buf, &len, &u16);
	narrow_len(&len, u16);

	debug("handshake header");
	expect(&buf, &len, "\x01", 1);
	get_u24(&buf, &len, &u32);
	narrow_len(&len, u32);

	debug("client version");
	skip(&buf, &len, 2);

	debug("client random");
	skip(&buf, &len, 32);

	debug("session id");
	get_u8(&buf, &len, &u8);
	skip(&buf, &len, u8);

	debug("cipher suites");
	get_u16(&buf, &len, &u16);
	skip(&buf, &len, u16);

	debug("compression methods");
	get_u8(&buf, &len, &u8);
	skip(&buf, &len, u8);

	debug("extension length");
	get_u16(&buf, &len, &u16);
	narrow_len(&len, u16);

	for (;;) {
		int ext_is_server_name;

		debug("extension type");
		get_u16(&buf, &len, &u16);
		ext_is_server_name = (u16 == 0x0000);

		debug("extension length");
		get_u16(&buf, &len, &u16);

		if (ext_is_server_name)
			break;
		skip(&buf, &len, u16);
	}

	debug("extension server name content");
	narrow_len(&len, u16);

	debug("extension list entry");
	get_u16(&buf, &len, &u16);
	narrow_len(&len, u16);

	debug("dns hostname");
	expect(&buf, &len, "\x00", 1);

	debug("extension server name string");
	get_u16(&buf, &len, &u16);
	narrow_len(&len, u16);

	memcpy(server_name, buf, len);
	server_name[len] = '\0';

	if (strlen(server_name) != len)
		default_name();

	debug("server_name=%s", server_name);
}

void
usage(void)
{
	fprintf(stderr, "usage: %s"
	  " [-e ENV=/path/%%s/file.pem]"
	  " cmd [arg...]"
	  "\n", conf.arg0);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct envfmt *list = NULL;
	char buf[1024], server_name[1024];
	ssize_t len;
	int c;

	conf.arg0 = *argv;
	while ((c = getopt(argc, argv, "e:")) > -1) {
		char *env, *fmt;

		if (c == '?')
			usage();

		if (envfmt_parse(optarg, &env, &fmt) < 0) {
			warn("invalid environment variable format");
			usage();
		}
		if (envfmt_add_new(&list, env, fmt) < 0)
			die("adding environment pattern to list");
	}
	conf.argv = argv + optind;
	if ((argc -= optind) == 0)
		usage();

	if ((len = recv(0, buf, sizeof(buf), MSG_PEEK)) == -1)
		die("first recv");

	parse(buf, server_name, len);

	if (setenv("SERVER_NAME", server_name, 1) == -1)
		die("setenv SERVER_NAME=%s", server_name);
	for (struct envfmt *ef = list; ef; ef = ef->next)
		if (envfmt_export(ef, server_name) < 0)
			die("setenv %s=%s", ef->env, ef->fmt);
	envfmt_free(list);

	execvp(conf.argv[0], conf.argv);
	die("execvp(2) into arguments");
	return 127;
}
