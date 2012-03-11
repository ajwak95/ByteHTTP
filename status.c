#include "httpd.h"
void status(int type, char *s1, int num)
{
	int fd;
	static char buffer[BUFSIZE*2];
	switch(type) {
	case 400: (void)sprintf(buffer, "<HTML><BODY><H1>400 Bad Request</H1></BODY><HTML>");
	break;
	case 403: (void)sprintf(buffer, "<HTML><BODY><H1>403 Forbidden</H1></BODY></HTML>");
	break;
	case 404: (void)sprintf(buffer, "<HTML><BODY><H1>404 Page Not Found</H1><ADDRESS>ByteHTTPD</ADDRESS></BODY></HTML>");
	break;
	}
	(void)write(num,buffer,strlen(buffer));

}
