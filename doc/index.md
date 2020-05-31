NotWiki
=======

[NotWiki](//code.z0.is/notwiki/) is a very simple site generator tool, that
support a simple subset of markdown in which this document is formatted:
NotMarkdown.

It supports publication of the same .md files for both http+html and gopher+gph.


How to generate a website?
--------------------------
The [notwiki-doc(1)][doc] tool will search for `*.md` files in all `$srcdir`
passed as arguments, and each file `$path/file.$ext` found, gets copied to the
matching `$dstdir/$path/file.$ext` directory.

	$ cd /home/me/website-document-root/
	$ notwiki-doc html /srv/www/htdocs/wiki ./wiki

[doc]: /man/notwiki-doc.1/

Here, `/home/me/website-document-root/wiki/introdcution/index.md` would be
copied to `/srv/www/htdocs/wiki/introduction/index.html`.

The original .md documents are copied along with the source, permitting the
wiki user to suggest modifications.

The `head.$ext` (`head.html`, `head.gph`) file is added at the top of the
converted document.


How does NotWiki work?
----------------------
It does not support editing files directly through the website: it is one shell
script calling one awk script on every page, generating a site in one of these
formats:

 * html - traditionnal format of the Web, through the notmarkdown-html backend.
 * gph - [geomyidae(1)](gopher://bitreich.org/1/scm/geomyidae/file/README.gph)'s
   format for Gopher, through the notmarkdow-gph backend.

To add a new converter backend, add in $PATH a script called notmarkdown-$ext
that reads NotMarkdown from stdin and sends the targetted format to stdout. For
instance, a notmarkdown-txt backend that print the document unchanged or a
notmarkdown-pdf aiming paper publication.


How to keep links compatible between HTTP and Gopher?
-----------------------------------------------------
Strip the protocol (http:, https:, gopher:) from all your own links, and eventually
remove the domain name as well.

`//example.com/wiki/` and `/wiki/` both get mapped to:

 * https://example.com/wiki/index.html (on web browser with HTTPS)
 * http://example.com/wiki/index.html (on web browser with HTTP)
 * gopher://example.com/wiki/index.gph (on gopher browsers)

So instead of /wiki/page-name.md, use /wiki/page-name/index.md, and use links
to `/wiki/page-name/` (with a trailing `/`, important for markdown-gph(1)).


How to handle vHosts on Gopher, like for the Web?
-------------------------------------------------
No vHosts on Gopher: multiple domains with the same destination point to the
same website, unlike the Web where you can redirect them as you wish.

For instance if `//doc.example.com/` and `//git.example.com/` point to the same
server, geomyidae(1) will pick the same `/index.gph` for both. A solution is to
always use prefixes, like `//doc.example.com/doc/` or `//git.example.com/git/`.


What can I do with it?
----------------------

### A software project wiki

This documentation is maintained in the `./doc/` directory of the git repo, and
on every commit, a [git-hook](//josuah.net/wiki/git-hooks/) regenerates the
documentation using NotWiki.

This permits to have the documentation bound to the code akin to Github's wikis
but self-hosted, and with zero-dependency (portable awk implementation):

	cd "$tmp/doc"
	notwiki-doc html "/srv/www/htdocs/code/notwiki" .
	notwiki-doc gph  "/srv/gopher/notwiki" .
	notwiki-mandoc gph utf8 "/srv/gopher/man" .
	notwiki-mandoc html html "/srv/www/htdocs/code/man" .

### A wiki for an existing website/gopherhole

It converts pages and copies them to the destination directory, which can
have content generated from multiple source. There is no `./index.html/gph`
overridden (unless you added a `./index.md` file yourself), so it will can be
mixed with content of another static generator.


Can I change the default theme?
-------------------------------
Yes, of course! In fact, there is no default theme, only an example provided
in the `./doc/` of the git repo of NotMarkdown, but all style, layout, header...
are provided by you in head.html or head.gph.

NotWiki websites can be something entirely different than documentation sites,
such as blogs, newspapers, association/enterprise presentation...


How does NotMarkdown differ with Markdown?
------------------------------------------
It is a fairly complete version of Markdown without nesting support, further
further described by [notmarkdown(5)](/man/notmarkdown.5/) man page.

There is a good support for escaping and `\`backtick\`` quoting.
