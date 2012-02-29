bytehttpd: httpd.o log.o web.o status.o
	cc httpd.o log.o web.o status.o -o bytehttpd
httpd.o: httpd.c
	cc -c httpd.c
web.o: web.c
	cc -c web.c
status.o: status.c
	cc -c status.c
log.o: log.c
	cc -c log.c
clean:
	rm -rf *o bytehttpd
