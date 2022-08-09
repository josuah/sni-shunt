sni-shunt
=========
`git://git.josuah.net/sni-shunt` - [0.3](/tool/sni-shunt/sni-shunt-0.3.tgz)

[sni-shunt(1)](/tool/sni-shunt/man/) is a small utility to
dispatch incoming TLS request to the apropriate backend according to the TLS
Server Name.

It is inspired by [calico](https://git.causal.agency/pounce/about/calico.1),
but calico only supports dispatching to UNIX domain sockets.

How to use it?
--------------
For instance, to choose the certificate file according to a pattern and then
starting the httpd within the connexion, using [s6-networking][s6] for handling
TCP and TLS and [httpfile][ht] for handling HTTP:

```
$ s6-tcpserver :: 443 \
  sni-shunt \
    -e KEYFILE=/etc/letsencrypt/live/%/privkey.pem \
    -e CERTFILE=/etc/letsencrypt/live/%/fullchain.pem \
  s6-tlsd \
  env UID="$(id -u www)" GID="$(id -g www)" ROOT="/srv/www/htdocs" \
  httpfile-httpd
```

[s6]: https://skarnet.org/software/s6-networking/
[ht]: https://mojzis.com/software/httpfile/

How to get it?
--------------
No dependency other than libc and cc: the TLS SNI parsing is not so hard, and  
built in sni-shunt. 

```
$ git clone git://code.z0.is/sni-shunt
$ cd sni-shunt
$ make PREFIX=/usr/local MANPREFIX=/usr/local/man install
```

What is it for?
---------------
TLS clients send the ServerName to the server while connecting to indicate
which certificate the server must send to it. sni-shunt uses this indication
to choose something different depending on the ServerName sent by the client.

This permits to shunt the incoming TLS connexion to a different backend 
according to the domain name used to connect, even if it is the same IP 
address, without any httpd or other protocol-specific server.

This permits to use tools like [s6-tlsserver]() for multiple certificates by 
specifying it which one to use, even though s6-tlsserver support only one.

How does it work?
-----------------
It expects to be run from [inetd(8)](https://en.wikipedia.org/wiki/Inetd)
or an [UCSPI](https://cr.yp.to/proto/ucspi.txt) server program such as
[s6-tcpserver](https://skarnet.org/software/s6-networking/s6-tcpserver.html)
so that it has a listenning socket at file descriptor 0 (`STDIN_FILENO`).

Once a client opens a TLS session, it peeks at the first 1024 bytes and parse
the Server Name value (no TLS library needed: rather simple parsing).

If found, it sets `SERVER_NAME` to the string encountered, as well as
others variables according to patterns sent via the `-e` command line flag.

It is transparent to the backend, as the bytes it reads are still considered
unread, due to [`MSG_PEEK` of recv(2)](http://man.openbsd.org/recv.2#MSG_PEEK)
so the back-end is still providing the TLS implementation. You read it right:
the ServerName message is parsed twice.

Any benchmark?
--------------
*sni-shunt* does not aim high-performance, so no benchmark.  But like calico,
it adds very little overhead as it does a very small task and let the TLS
handling to the child program.
