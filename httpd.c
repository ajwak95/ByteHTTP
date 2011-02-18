
/*     This file is part of SlimHTTPD.

    SlimHTTPD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SlimHTTPD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SlimHTTPD.  If not, see <http://www.gnu.org/licenses/>.
-------------------
    httpd.c       |
   Alex Conrey    |
   2-16-11        |
-------------------
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BACKLOG 4
#define HTTPD_PORT 80
#define CONFIG_PATH "httpd.conf"
#define HTTPD_ROOT "index.html"
#define HTTPD_PROTOCOL "HTTP/1.1"
void Conf() {
  char line[256];
  int linenum=0;
  FILE *config;
  config = fopen(CONFIG_PATH, "r");
  while(fgets(line, 256, config) != NULL) {
    char root[256], path[256];
    linenum++;
    if(line[0] == '#') continue;

    if(sscanf(line, "%s %s", root, path) != 2)
      {
	fprintf(stderr, "Syntax error, line %d\n", linenum);
	continue;
      }
    printf("%s = %s\n", root, path);
  }
}
void Headers() {
  HTTPD_PROTOCOL;
}
int main(char *argv[]) {
  Conf();  
  register int s, c;
    int b;
    struct sockaddr_in sa;
    struct tm *tm;
    FILE *client; // Client connection
    FILE *tbserv; // (T)o (B)e (Serv)ed
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    bzero(&sa, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port   = htons(HTTPD_PORT);

    if (INADDR_ANY)
        sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&sa, sizeof sa) < 0) {
        perror("bind");
        return 2;
    }
    // Fork daemon to background
    switch (fork()) {
        case -1:
            perror("fork");
            return 3;
            break;
        default:
            close(s);
            return 0;
            break;
        case 0:
            break;
	    }

    listen(s, BACKLOG);

    for (;;) {
        b = sizeof sa;

        if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) {
            perror("httpd");
            return 4;
        }

        if ((client = fdopen(c, "w")) == NULL) {
            perror("httpd fdopen");
            return 5;
        }
	char v[256];
	int f;
	if((tbserv=fopen(HTTPD_ROOT, "r")) == NULL) {
	  fprintf(client, "Cannot open file.\n");
	}
	// This way the loaded file can have more than 1 word..
	while(fscanf(tbserv, "%s", v) == 1)
	  fprintf(client, "%s\n", v);
	
	fclose(tbserv);
	fclose(client);
    }
}
