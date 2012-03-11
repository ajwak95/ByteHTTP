#include "httpd.h"
void directory(char *path, int num)
{
	DIR *dp = opendir(path);
	int fd;
	static char buffer[BUFSIZE*2];
	  struct dirent *ep;
	  if (dp != NULL)
	  {
		  (void)sprintf(buffer, "<HTML><BODY><H1>Index Listing</H1>");

	    while (ep = readdir(dp))
	      sprintf(buffer, "%s", ep->d_name);

	      (void) closedir (dp);

	  }
	  (void)sprintf(buffer, "</BODY></HTML>");
	  (void)write(num, buffer, strlen(buffer));

}
