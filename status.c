#include "httpd.h"
void status(int type, char *s1, int num)
{
	int fd;
	static char buffer[BUFSIZE*2];
	switch(type) {
	case 404: (void)sprintf(buffer, "<HTML><BODY><H1>404 Page Not Found</H1><ADDRESS>ByteHTTPD</ADDRESS></BODY></HTML>");
	(void)write(num,buffer,strlen(buffer));
	break;
	}

}
