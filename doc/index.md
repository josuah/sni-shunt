sni-shunt
=========

[sni-shunt](//code.z0.is/sni-shunt/) is a small utility to dispatch incoming
TLS request to the apropriate backend according to the TLS Server Name.

It is inspired by [calico](https://git.causal.agency/pounce/about/calico.1),
with the same approach, but only supports dispatching to UNIX domain sockets.

It is expected to be run from [inetd(8)](https://en.wikipedia.org/wiki/Inetd)
or an [UCSPI](https://cr.yp.to/proto/ucspi.txt) server program such as
[s6-tcpserver](https://skarnet.org/software/s6-networking/s6-tcpserver.html)
so that it has a listenning socket at file descriptor 0 (`STDIN_FILENO`).

Once a client opens a TLS session, it peeks at the first 1024 bytes and parse
the Server Name value (no TLS library needed for that).

If found, it sets the `SERVER_NAME` to the string encountered, as well as 
others according to patterns sent via command line flags.
