cgisysinfo
----------

cgisysinfo is a small CGI utility to show basic system information on Linux
machines.

You just need to use the main binary ``cgisysinfo`` as your CGI program.

cgisysinfo is useful for machines that run a webserver but not PHP, and was
tested with nginx + fcgiwrap. It should works with any CGI-capable web server.

Additionaly, cgisysinfo may be called directly from the command-line, and will
print a HTML file to stdout. You can setup a cron job to run cgisysinfo and
create the HTML file inside of your web server's document root, without use
CGI.

cgisysinfo just works on Linux. ;)

The binary doesn't have any options and relies on the information from the
``/proc`` pseudo-file system and some environment variables.

There's an experimental support for FastCGI available since cgisysinfo-0.2.
To enable it, please configure cgisysinfo with ``--enable-fastcgi``.

Enjoy!

Rafael G. Martins <rafael@rafaelmartins.eng.br>
