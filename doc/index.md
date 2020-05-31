sni-shunt
=========

[sni-shunt(1)](//code.z0.is/man/sni-shunt.1) is a small utility to
dispatch incoming TLS request to the apropriate backend according to the TLS
Server Name.

It is inspired by [calico](https://git.causal.agency/pounce/about/calico.1),
with the same approach, but only supports dispatching to UNIX domain sockets.

It is expected to be run from [inetd(8)](https://en.wikipedia.org/wiki/Inetd)
or an [UCSPI](https://cr.yp.to/proto/ucspi.txt) server program such as
[s6-tcpserver](https://skarnet.org/software/s6-networking/s6-tcpserver.html)
so that it has a listenning socket at file descriptor 0 (`STDIN_FILENO`).

Once a client opens a TLS session, it peeks at the first 1024 bytes and parse
the Server Name value (no TLS library needed for that).

If found, it sets the `SERVER_NAME` to the string encountered, as well as 
others variables according to patterns sent via the `-e` command line flag.

For instance, to choose the certificate file according to a pattern and then
starting the httpd within the connexion:

```
$ s6-tcpserver :: 443 \
  sni-shunt \
    -e KEYFILE=/etc/letsencrypt/live/%/privkey.pem \
    -e CERTFILE=/etc/letsencrypt/live/%/fullchain.pem \
  s6-tlsd \
  env UID="$(id -u www)" GID="$(id -g www)" ROOT="/srv/www/htdocs" \
  httpfile-httpd
```
